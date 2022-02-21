/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "liquidlistmodel.h"
#include "../objects/liquid_p.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

LiquidListModel::LiquidListModel(QObject *parent) : BaseListModel(parent)
{

}

LiquidListModel::~LiquidListModel()
{

}

bool LiquidListModel::load()
{
    setInOperation(true);

    qDebug("Loading liquids for person ID %i on %s", personId(), qUtf8Printable(day().toString()));

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

    const QDateTime start = QDateTime(day(), dayStarts());
    const QDateTime end = QDateTime(day().addDays(1), dayStarts());

    QSqlQuery q;

    if (Q_UNLIKELY(!q.prepare(QStringLiteral("SELECT id, person_id, moment, in_or_out, amount, name, note FROM liquid WHERE person_id = :person_id AND moment >= :start AND moment < :end ORDER BY moment DESC")))) {
        setLastError(qtTrId("naz-err-failed-prepare-db-query").arg(q.lastError().text()));
        qCritical("Failed to prepare database query: %s", qUtf8Printable(q.lastError().text()));
        return 0;
    }

    q.bindValue(QStringLiteral(":person_id"), personId());
    q.bindValue(QStringLiteral(":start"), start);
    q.bindValue(QStringLiteral(":end"), end);

    if (Q_UNLIKELY(!q.exec())) {
        setLastError(qtTrId("naz-err-failed-execute-db-query").arg(q.lastError().text()));
        qCritical("Failed to execute database query: %s", qUtf8Printable(q.lastError().text()));
        setInOperation(false);
        return false;
    }

    std::vector<Liquid> _liquids;
    int diff = 0;
    while (q.next()) {
        const Liquid::InOrOut inOrOut = static_cast<Liquid::InOrOut>(q.value(3).toInt());
        const int amount = q.value(4).toInt();
        _liquids.emplace_back(q.value(0).toInt(),
                              q.value(1).toInt(),
                              q.value(2).toDateTime(),
                              inOrOut,
                              amount,
                              q.value(5).toString(),
                              q.value(6).toString());
        diff += (inOrOut == Liquid::Out ? (-1 * amount) : amount);
    }

    setDifference(diff);

    if (!_liquids.empty()) {
        beginInsertRows(QModelIndex(), 0, _liquids.size() - 1);
        m_liquids = _liquids;
        endInsertRows();
    }

    setInOperation(false);

    return true;
}

bool LiquidListModel::loadForPerson(int personId,  QTime dayStarts, QDate day)
{
    setPersonId(personId);
    setDayStarts(dayStarts);
    setDay(day);
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

int LiquidListModel::add(int id, const QDateTime &moment, int inOrOut, int amount, const QString &name, const QString &note)
{
    const QDate _day = moment.date();
    const QTime _time = moment.time();
    const QDate _liquidDay = _time >= dayStarts() ? _day : _day.addDays(-1);

    if (_liquidDay != day()) {
        return 0;
    }

    const Liquid::InOrOut _inOrOut = static_cast<Liquid::InOrOut>(inOrOut);

    if (_inOrOut == Liquid::Out) {
        setDifference(difference() + (-1 * amount));
    } else {
        setDifference(difference() + amount);
    }

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

bool LiquidListModel::remove(QModelIndex index)
{
    if (!index.isValid() || (index.row() >= rowCount())) {
        qWarning("%s", "Invalid index");
        return false;
    }

    const Liquid l = m_liquids.at(index.row());

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

    if (l.inOrOut() == Liquid::Out) {
        setDifference(difference() - (-1 * l.amount()));
    } else {
        setDifference(difference() - l.amount());
    }

    qDebug("Removed Liquid %i ml %s with ID %i", l.amount(), qUtf8Printable(l.name()), l.id());

    return true;
}

bool LiquidListModel::edit(QModelIndex index, const QDateTime &moment, int inOrOut, int amount, const QString &name, const QString &note)
{
    if (!index.isValid() || index.row() >= rowCount()) {
        qWarning("Invalid index: %i", index.row());
        return false;
    }

    if (!setData(index, moment, Moment)) {
        return false;
    }

    if (!setData(index, inOrOut, InOrOut)) {
        return false;
    }

    if (!setData(index, amount, Amount)) {
        return false;
    }

    if (!setData(index, name, Name)) {
        return false;
    }

    if (!setData(index, note, Note)) {
        return false;
    }

    const QDate _day = moment.date();
    const QTime _time = moment.time();
    const QDate _liquidDay = _time >= dayStarts() ? _day : _day.addDays(-1);

    if (_liquidDay != day()) {
        beginRemoveRows(QModelIndex(), index.row(), index.row());
        m_liquids.erase(m_liquids.begin() + index.row());
        endRemoveRows();
        updateDifference();
    }

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

    const auto l = m_liquids.at(index.row());

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
        qWarning("Invalid index: %i", index.row());
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

    if (role == InOrOut || role == Amount) {
        updateDifference();
    }

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

QTime LiquidListModel::dayStarts() const
{
    return m_dayStarts;
}

void LiquidListModel::setDayStarts(QTime start)
{
    if (m_dayStarts != start) {
        qDebug("Changing dayStarts from \"%s\" to \"%s\"", qUtf8Printable(m_dayStarts.toString()), qUtf8Printable(start.toString()));
        m_dayStarts = start;
        emit dayStartsChanged(dayStarts());
    }
}

QDate LiquidListModel::day() const
{
    return m_day;
}

void LiquidListModel::setDay(QDate day)
{
    if (m_day != day) {
        qDebug("Changing day from \"%s\" to \"%s\"", qUtf8Printable(m_day.toString()), qUtf8Printable(day.toString()));
        m_day = day;
        emit dayChanged(this->day());
    }
}

int LiquidListModel::difference() const
{
    return m_difference;
}

void LiquidListModel::setDifference(int difference)
{
    if (m_difference != difference) {
        qDebug("Changing difference from %i to %i", m_difference, difference);
        m_difference = difference;
        emit differenceChanged(this->difference());
    }
}

void LiquidListModel::updateDifference()
{
    int diff = 0;
    for (const auto &l : m_liquids) {
        diff += (l.inOrOut() == Liquid::Out ? (-1 * l.amount()) : l.amount());
    }
    setDifference(diff);
}

#include "moc_liquidlistmodel.cpp"
