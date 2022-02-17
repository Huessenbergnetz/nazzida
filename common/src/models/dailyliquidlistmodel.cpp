/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "dailyliquidlistmodel.h"
#include "../objects/dailyliquids_p.h"
#include "../objects/liquid.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

DailyLiquidListModel::DailyLiquidListModel(QObject *parent) : BaseListModel(parent)
{

}

DailyLiquidListModel::~DailyLiquidListModel()
{

}

bool DailyLiquidListModel::load()
{
    setInOperation(true);

    qDebug("Loading daily liquids for person ID %i", personId());

    {
        QSqlDatabase db = QSqlDatabase::database();
        if (!db.open()) {
            setLastError(qtTrId("naz-err-failed-open-db").arg(db.databaseName(), db.lastError().text()));
            qCritical("Failed to open database %s: %s", qUtf8Printable(db.databaseName()), qUtf8Printable(db.lastError().text()));
            setInOperation(false);
            return false;
        }
    }

    clear();

    QSqlQuery q;

    if (Q_UNLIKELY(!q.prepare(QStringLiteral("SELECT id, person_id, moment, in_or_out, amount FROM liquid WHERE person_id = :person_id ORDER BY moment DESC")))) {
        setLastError(qtTrId("naz-err-failed-prepare-db-query").arg(q.lastError().text()));
        qCritical("Failed to prepare database query: %s", qUtf8Printable(q.lastError().text()));
        return 0;
    }

    q.bindValue(QStringLiteral(":person_id"), personId());

    if (Q_UNLIKELY(!q.exec())) {
        setLastError(qtTrId("naz-err-failed-execute-db-query").arg(q.lastError().text()));
        qCritical("Failed to execute database query: %s", qUtf8Printable(q.lastError().text()));
        setInOperation(false);
        return false;
    }

    std::vector<DailyLiquids> _liquids;

    while (q.next()) {
        const int id = q.value(0).toInt();
        const int personId = q.value(1).toInt();
        const QDateTime moment = q.value(2).toDateTime();
        const Liquid::InOrOut inOrOut = static_cast<Liquid::InOrOut>(q.value(3).toInt());
        const int amount = q.value(4).toInt();

        const QDate day = moment.date();
        const QTime time = moment.time();

        QDate liquidDay; // the day the values belong to
        if (time >= dayStarts()) {
            liquidDay = day;
        } else {
            liquidDay = day.addDays(-1);
        }

        int idx = -1;
        int currentIndex = 0;
        for (const auto &l : _liquids) {
            if (l.day() == liquidDay) {
                idx = currentIndex;
                break;
            } else {
                currentIndex++;
            }
        }

        if (idx > -1) {
            auto ld = _liquids.at(idx);
            if (inOrOut == Liquid::Out) {
                ld.d->output = ld.output() + amount;
            } else {
                ld.d->input = ld.input() + amount;
            }
            _liquids[idx] = ld;
        } else {
            const int input = inOrOut == Liquid::Out ? 0 : amount;
            const int output = inOrOut == Liquid::Out ? amount : 0;
            _liquids.emplace_back(id, personId, liquidDay, input, output);
        }
    }

    if (!_liquids.empty()) {
        beginInsertRows(QModelIndex(), 0, _liquids.size() - 1);
        m_dailyLiquids = _liquids;
        endInsertRows();
    }

    setInOperation(false);

    return true;
}

bool DailyLiquidListModel::loadForPerson(int personId, QTime dayStarts)
{
    setPersonId(personId);
    setDayStarts(dayStarts);
    return load();
}

void DailyLiquidListModel::clear()
{
    if (!m_dailyLiquids.empty()) {
        beginRemoveRows(QModelIndex(), 0, m_dailyLiquids.size() - 1);
        m_dailyLiquids.clear();
        endRemoveRows();
    }
}

int DailyLiquidListModel::add(const QDateTime &moment, int inOrOut, int amount, const QString &name, const QString &note)
{
    Liquid::InOrOut _inOrOut = static_cast<Liquid::InOrOut>(inOrOut);

    QSqlQuery q;

    if (!q.exec(QStringLiteral("PRAGMA foreign_keys = ON"))) {
        setLastError(qtTrId("naz-err-failed-foreign-keys-pragma").arg(q.lastError().text()));
        qCritical("Failed to enable foreign keys pragma: %s", qUtf8Printable(q.lastError().text()));
        return 0;
    }

    if (Q_UNLIKELY(!q.prepare(QStringLiteral("INSERT INTO liquid (person_id, moment, in_or_out, amount, name, note) VALUES (:person_id, :moment, :in_or_out, :amount, :name, :note)")))) {
        setLastError(qtTrId("naz-err-failed-prepare-db-query").arg(q.lastError().text()));
        qCritical("Failed to prepare database query: %s", qUtf8Printable(q.lastError().text()));
        return 0;
    }

    q.bindValue(QStringLiteral(":person_id"), personId());
    q.bindValue(QStringLiteral(":moment"), moment);
    q.bindValue(QStringLiteral(":in_or_out"), _inOrOut);
    q.bindValue(QStringLiteral(":amount"), amount);
    q.bindValue(QStringLiteral(":name"), name);
    q.bindValue(QStringLiteral(":note"), note);

    if (Q_UNLIKELY(!q.exec())) {
        setLastError(qtTrId("naz-err-failed-execute-db-query").arg(q.lastError().text()));
        qCritical("Failed to execute database query: %s", qUtf8Printable(q.lastError().text()));
        return 0;
    }

    const int id = q.lastInsertId().toInt();

    const QDate day = moment.date();
    const QTime time = moment.time();
    const QDate liquidDay = time >= dayStarts() ? day : day.addDays(-1);

    int idx = -1;
    int currentIndex = 0;
    for (auto &l : m_dailyLiquids) {
        if (l.day() == liquidDay) {
            idx = currentIndex;
            break;
        } else {
            currentIndex++;
        }
    }

    if (idx > -1) {
        auto l = m_dailyLiquids.at(idx);
        if (_inOrOut == Liquid::Out) {
            l.d->output = l.output() + amount;
        } else {
            l.d->input = l.input() + amount;
        }
        m_dailyLiquids[idx] = l;
        emit dataChanged(index(idx, 0), index(idx, 0), {DailyLiquidListModel::Input, DailyLiquidListModel::Output, DailyLiquidListModel::Difference});
    } else {
        const int input = inOrOut == Liquid::Out ? 0 : amount;
        const int output = inOrOut == Liquid::Out ? amount : 0;
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_dailyLiquids.emplace_back(id, personId(), liquidDay, input, output);
        endInsertRows();
    }

    qDebug("Created new liquid %i ml %s with ID %i for person ID %i", amount, qUtf8Printable(name), id, personId());

    return id;
}

QModelIndex DailyLiquidListModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}

QVariant DailyLiquidListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() >= rowCount())) {
        qWarning("Invalid model index: %i", index.row());
        return QVariant();
    }

    const auto l = m_dailyLiquids.at(index.row());

    switch (role) {
    case Id:
        return l.id();
    case PersonId:
        return l.personId();
    case Day:
        return l.day();
    case Input:
        return l.input();
    case Output:
        return l.output();
    case Difference:
        return l.difference();
    default:
        return QVariant();
    }
}

int DailyLiquidListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_dailyLiquids.size();
}

QHash<int, QByteArray> DailyLiquidListModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(Id, QByteArrayLiteral("id"));
    roles.insert(PersonId, QByteArrayLiteral("personId"));
    roles.insert(Day, QByteArrayLiteral("day"));
    roles.insert(Input, QByteArrayLiteral("input"));
    roles.insert(Output, QByteArrayLiteral("output"));
    roles.insert(Difference, QByteArrayLiteral("difference"));
    return roles;
}

int DailyLiquidListModel::personId() const
{
    return m_personId;
}

void DailyLiquidListModel::setPersonId(int id)
{
    if (m_personId != id) {
        qDebug("Changing personId from %i to %i", m_personId, id);
        m_personId = id;
        emit personIdChanged(personId());
    }
}

QTime DailyLiquidListModel::dayStarts() const
{
    return m_dayStarts;
}

void DailyLiquidListModel::setDayStarts(QTime dayStarts)
{
    if (m_dayStarts != dayStarts) {
        qDebug("Changing dayStarts from \"%s\" to \"%s\"", qUtf8Printable(m_dayStarts.toString()), qUtf8Printable(dayStarts.toString()));
        m_dayStarts = dayStarts;
        emit dayStartsChanged(this->dayStarts());
    }
}

#include "moc_dailyliquidlistmodel.cpp"
