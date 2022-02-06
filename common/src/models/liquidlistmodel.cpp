/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "liquidlistmodel.h"
#include "../objects/liquid_p.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

LiquidListModel::LiquidListModel(QObject *parent) : BaseListModel(parent)
{

}

LiquidListModel::~LiquidListModel()
{

}

bool LiquidListModel::load()
{
    setInOperation(true);

    qDebug("Loading liquids for person ID %i", personId());

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

    if (Q_UNLIKELY(!q.prepare(QStringLiteral("SELECT id, person_id, moment, in_or_out, amount, name, note FROM liquid WHERE person_id = :person_id ORDER BY moment DESC")))) {
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

    std::vector<Liquid> _liquids;
    while (q.next()) {
        _liquids.emplace_back(q.value(0).toInt(),
                              q.value(1).toInt(),
                              q.value(2).toDateTime(),
                              static_cast<Liquid::InOrOut>(q.value(3).toInt()),
                              q.value(4).toInt(),
                              q.value(5).toString(),
                              q.value(6).toString());
    }

    if (!_liquids.empty()) {
        beginInsertRows(QModelIndex(), 0, _liquids.size() - 1);
        m_liquids = _liquids;
        endInsertRows();
    }

    setInOperation(false);

    return true;
}

bool LiquidListModel::loadForPerson(int personId)
{
    setPersonId(personId);
    return load();
}

void LiquidListModel::clear()
{
    if (!m_liquids.empty()) {
        beginRemoveRows(QModelIndex(), 0, m_liquids.size() - 1);
        m_liquids.clear();
        endRemoveRows();
    }
}

int LiquidListModel::add(const QDateTime &moment, int inOrOut, int amount, const QString &name, const QString &note)
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

    beginInsertRows(QModelIndex(), m_liquids.size(), m_liquids.size());

    m_liquids.emplace_back(id,
                           personId(),
                           moment,
                           _inOrOut,
                           amount,
                           name,
                           note);

    endInsertRows();

    qDebug("Created new liquid %i ml %s with ID %i for person ID %i", amount, qUtf8Printable(name), id, personId());

    return id;
}

bool LiquidListModel::remove(const QModelIndex &index)
{
    if (!index.isValid() || (index.row() >= rowCount())) {
        qWarning("%s", "Invalid index");
        return false;
    }

    Liquid l = m_liquids.at(index.row());

    QSqlQuery q;

    if (Q_UNLIKELY(!q.prepare(QStringLiteral("DELETE FROM liquid WHERE id = :id")))) {
        setLastError(qtTrId("naz-err-failed-prepare-db-query").arg(q.lastError().text()));
        qCritical("Failed to prepare database query: %s", qUtf8Printable(q.lastError().text()));
        return false;
    }

    q.bindValue(QStringLiteral(":id"), l.id());

    if (Q_UNLIKELY(!q.exec())) {
        setLastError(qtTrId("naz-err-failed-execute-db-query").arg(q.lastError().text()));
        qCritical("Failed to execute database query: %s", qUtf8Printable(q.lastError().text()));
        return false;
    }

    beginRemoveRows(QModelIndex(), index.row(), index.row());

    m_liquids.erase(m_liquids.begin() + index.row());

    endRemoveRows();

    qDebug("Removed Liquid %i ml %s with ID %i", l.amount(), qUtf8Printable(l.name()), l.id());

    return true;
}

QModelIndex LiquidListModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}

QVariant LiquidListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() >= rowCount())) {
        return QVariant();
    }

    auto l = m_liquids.at(index.row());

    switch(role) {
    case Id:
        return l.id();
    case PersonId:
        return l.personId();
    case Moment:
        return l.moment();
    case InOrOut:
        return l.inOrOut();
    case Amount:
        return l.amount();
    case Name:
        return l.name();
    case Note:
        return l.note();
    default:
        return QVariant();
    }
}

int LiquidListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_liquids.size();
}

QHash<int, QByteArray> LiquidListModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(Id, QByteArrayLiteral("id"));
    roles.insert(PersonId, QByteArrayLiteral("personId"));
    roles.insert(Moment, QByteArrayLiteral("moment"));
    roles.insert(InOrOut, QByteArrayLiteral("inOrOut"));
    roles.insert(Amount, QByteArrayLiteral("amount"));
    roles.insert(Name, QByteArrayLiteral("name"));
    roles.insert(Note, QByteArrayLiteral("note"));
    return roles;
}

bool LiquidListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || (index.row() >= rowCount())) {
        qWarning("%s", "Invalid index");
        return false;
    }

    Liquid l = m_liquids.at(index.row());
    QString roleCol;

    switch(role) {
    case Id:
    case PersonId:
    {
        qWarning("Can not change id or person id");
        return false;
    }
    case Moment:
    {
        if (l.moment() == value.toDateTime()) {
            return true;
        }
        roleCol = QStringLiteral("moment");
    }
        break;
    case InOrOut:
    {
        if (l.inOrOut() == static_cast<Liquid::InOrOut>(value.toInt())) {
            return true;
        }
        roleCol = QStringLiteral("in_or_out");
    }
        break;
    case Amount:
    {
        if (l.amount() == value.toInt()) {
            return true;
        }
        roleCol = QStringLiteral("amount");
    }
        break;
    case Name:
    {
        if (l.name() == value.toString()) {
            return true;
        }
        roleCol = QStringLiteral("name");
    }
        break;
    case Note:
    {
        if (l.note() == value.toString()) {
            return true;
        }
        roleCol = QStringLiteral("note");
    }
        break;
    default:
    {
        qWarning("Invalid role: %i", role);
        return false;
    }
    }

    QSqlQuery q;

    if (Q_UNLIKELY(!q.prepare(QStringLiteral("UPDATE liquid SET %1 = :value WHERE id = :id").arg(roleCol)))) {
        setLastError(qtTrId("naz-err-failed-prepare-db-query").arg(q.lastError().text()));
        qCritical("Failed to prepare database query: %s", qUtf8Printable(q.lastError().text()));
        return false;
    }

    q.bindValue(QStringLiteral(":value"), value);
    q.bindValue(QStringLiteral(":id"), l.id());

    if (Q_UNLIKELY(!q.exec())) {
        setLastError(qtTrId("naz-err-failed-execute-db-query").arg(q.lastError().text()));
        qCritical("Failed to execute database query: %s", qUtf8Printable(q.lastError().text()));
        return false;
    }

    switch(role) {
    case Moment:
        l.d->moment = value.toDateTime();
        break;
    case InOrOut:
        l.d->inOrOut = static_cast<Liquid::InOrOut>(value.toInt());
        break;
    case Amount:
        l.d->amount = value.toInt();
        break;
    case Name:
        l.d->name = value.toString();
        break;
    case Note:
        l.d->note = value.toString();
        break;
    default:
    {
        qWarning("Invalid role %i", role);
        return false;
    }
    }

    m_liquids[index.row()] = l;

    emit dataChanged(index, index, {role});

    return true;
}

int LiquidListModel::personId() const
{
    return m_personId;
}

void LiquidListModel::setPersonId(int id)
{
    if (m_personId != id) {
        qDebug("Changing personId from %i to %i", m_personId, id);
        m_personId = id;
        emit personIdChanged(personId());
    }
}

#include "moc_liquidlistmodel.cpp"
