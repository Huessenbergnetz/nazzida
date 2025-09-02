/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "bloodpressurelistmodel.h"
#include "../objects/bloodpressure_p.h"

#include <QDateTime>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

BloodPressureListModel::BloodPressureListModel(QObject *parent)
    : BaseListModel(parent)
{
}

BloodPressureListModel::~BloodPressureListModel() = default;

bool BloodPressureListModel::load()
{
    setInOperation(true);

    qDebug() << "Loading blood pressure and pulse data for person ID" << personId();

    {
        auto db = QSqlDatabase::database();
        if (Q_UNLIKELY(!db.open())) {
            setLastError(qtTrId("naz-err-failed-open-db").arg(db.databaseName(), db.lastError().text()));
            qCritical().nospace() << "Failed to open database " << db.databaseName() << ": " << db.lastError();
            setInOperation(false);
            return false;
        }
    }

    clear();

    QSqlQuery q;

    if (Q_UNLIKELY(!q.prepare(QStringLiteral("SELECT id, moment, systolic, diastolic, pulse, note FROM bloodpressure WHERE person_id = :person_id ORDER BY moment DESC")))) {
        // defined at peoplelistmodel.cpp
        setLastError(qtTrId("naz-err-failed-prepare-db-query").arg(q.lastError().text()));
        qCritical() << "Failed to prepare database query:" << q.lastError();
        setInOperation(false);
        return false;
    }

    q.bindValue(QStringLiteral(":person_id"), personId());

    if (Q_UNLIKELY(!q.exec())) {
        // defined at peoplelistmode.cpp
        setLastError(qtTrId("naz-err-failed-execute-db-query").arg(q.lastError().text()));
        qCritical() << "Failde to execute database query:" << q.lastError();
        setInOperation(false);
        return false;
    }

    std::vector<BloodPressure> _bloodPressures;
    while (q.next()) {
        auto momentUtc = q.value(1).toDateTime();
        momentUtc.setTimeSpec(Qt::UTC);

        _bloodPressures.emplace_back(q.value(0).toInt(),
                                     personId(),
                                     momentUtc.toLocalTime(),
                                     q.value(2).toInt(),
                                     q.value(3).toInt(),
                                     q.value(4).toInt(),
                                     q.value(5).toString());
    }

    if (!_bloodPressures.empty()) {
        beginInsertRows(QModelIndex(), 0, _bloodPressures.size() -1 );
        m_bloodPressures = _bloodPressures;
        endInsertRows();
    }

    setInOperation(false);

    return true;
}

bool BloodPressureListModel::loadForPerson(int personId)
{
    setPersonId(personId);
    return load();
}

void BloodPressureListModel::clear()
{
    if (!m_bloodPressures.empty()) {
        beginRemoveRows(QModelIndex(), 0, m_bloodPressures.size() - 1);
        m_bloodPressures.clear();
        endRemoveRows();
    }
}

bool BloodPressureListModel::add(const QDateTime &moment, int systolic, int diastolic, int pulse, const QString &note)
{
    QSqlQuery q;

    if (Q_UNLIKELY(!q.exec(QStringLiteral("PRAGMA foreign_keys = ON")))) {
        setLastError(qtTrId("naz-err-failed-foreign-keys-pragma").arg(q.lastError().text()));
        qCritical() << "Failed to enable foreign keys pragma:" << q.lastError();
        return false;
    }

    if (Q_UNLIKELY(!q.prepare(QStringLiteral("INSERT INTO bloodpressure (person_id, moment, systolic, diastolic, pulse, note) "
                                             "VALUES (:person_id, :moment, :systolic, :diastolic, :pulse, :note)")))) {
        setLastError(qtTrId("naz-err-failed-prepare-db-query").arg(q.lastError().text()));
        qCritical() << "Failed to prepare database query:" << q.lastError();
        return false;
    }

    q.bindValue(QStringLiteral(":person_id"), personId());
    q.bindValue(QStringLiteral(":moment"), moment.toUTC());
    q.bindValue(QStringLiteral(":systolic"), systolic);
    q.bindValue(QStringLiteral(":diastolic"), diastolic);
    q.bindValue(QStringLiteral(":pulse"), pulse);
    q.bindValue(QStringLiteral(":note"), note);

    if (Q_UNLIKELY(!q.exec())) {
        setLastError(qtTrId("naz-err-failed-execute-db-query").arg(q.lastError().text()));
        qCritical() << "Failed to execute database query:" << q.lastError();
        return false;
    }

    const int id = q.lastInsertId().toInt();

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_bloodPressures.emplace_back(id, personId(), moment, systolic, diastolic, pulse, note);
    endInsertRows();

    qDebug().nospace() << "Created new blood pressure data " << systolic << "/" << diastolic << ", pulse: " << pulse << " for person ID " << personId();

    return true;
}

bool BloodPressureListModel::remove(QModelIndex index)
{
    if (!index.isValid() || (index.row() >= rowCount())) {
        qWarning() << "Invalid" << index;
        return false;
    }

    const BloodPressure bp = m_bloodPressures.at(index.row());

    QSqlQuery q;

    if (Q_UNLIKELY(!q.prepare(QStringLiteral("DELETE FROM bloodpressure WHERE id = :id")))) {
        setLastError(qtTrId("naz-err-failed-prepare-db-query").arg(q.lastError().text()));
        qCritical() << "Failed to prepare database query:" << q.lastError();
        return false;
    }

    q.bindValue(QStringLiteral(":id"), bp.id());

    if (Q_UNLIKELY(!q.exec())) {
        setLastError(qtTrId("naz-err-failed-execute-db-query").arg(q.lastError().text()));
        qCritical() << "Failed to execute database query:" << q.lastError();
        return false;
    }

    beginRemoveRows(QModelIndex(), index.row(), index.row());

    m_bloodPressures.erase(m_bloodPressures.begin() + index.row());

    endRemoveRows();

    qDebug().nospace() << "Removed blood pressure data " << bp.systolic() << "/" << bp.diastolic()
                       << ", pulse: " << bp.pulse() << ". Recorded at " << bp.moment()
                       << " from person ID " << bp.personId();

    return true;
}

bool BloodPressureListModel::edit(QModelIndex index, const QDateTime &moment, int systolic, int diastolic, int pulse, const QString &note)
{
    if (!index.isValid() || (index.row() >= rowCount())) {
        qWarning() << "Invalid" << index;
        return false;
    }

    if (!setData(index, moment, Moment)) {
        return false;
    }

    if (!setData(index, systolic, Systolic)) {
        return false;
    }

    if (!setData(index, diastolic, Diastolic)) {
        return false;
    }

    if (!setData(index, pulse, Pulse)) {
        return false;
    }

    if (!setData(index, note, Note)) {
        return false;
    }

    return true;
}

QModelIndex BloodPressureListModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}

QVariant BloodPressureListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() >= rowCount())) {
        qWarning() << "Invalid" << index;
        return false;
    }

    const auto bp = m_bloodPressures.at(index.row());

    switch(role) {
    case Id:
        return bp.id();
    case PersonId:
        return bp.personId();
    case Moment:
        return bp.moment();
    case Systolic:
        return bp.systolic();
    case Diastolic:
        return bp.diastolic();
    case Pulse:
        return bp.pulse();
    case Note:
        return bp.note();
    case RelDate:
        return bp.relDate();
    }

    return QVariant();
}

int BloodPressureListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_bloodPressures.size();
}

QHash<int, QByteArray> BloodPressureListModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(Id, QByteArrayLiteral("id"));
    roles.insert(PersonId, QByteArrayLiteral("personId"));
    roles.insert(Moment, QByteArrayLiteral("moment"));
    roles.insert(Systolic, QByteArrayLiteral("systolic"));
    roles.insert(Diastolic, QByteArrayLiteral("diastolic"));
    roles.insert(Pulse, QByteArrayLiteral("pulse"));
    roles.insert(Note, QByteArrayLiteral("note"));
    roles.insert(RelDate, QByteArrayLiteral("relDate"));
    return roles;
}

bool BloodPressureListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || (index.row() >= rowCount())) {
        qWarning() << "Invalid" << index;
        return false;
    }

    BloodPressure bp = m_bloodPressures.at(index.row());

    QString roleCol;
    QVariant _value = value;

    switch(role) {
    case Id:
    case PersonId:
    case RelDate:
        qWarning() << "Can not change id, personId or relDate";
        return false;
    case Moment:
    {
        if (bp.moment() == value.toDateTime()) {
            return true;
        }
        roleCol = QStringLiteral("moment");
        _value = value.toDateTime().toUTC();
    }
        break;
    case Systolic:
    {
        if (bp.systolic() == value.toInt()) {
            return true;
        }
        roleCol = QStringLiteral("systolic");
    }
        break;
    case Diastolic:
    {
        if (bp.diastolic() == value.toInt()) {
            return true;
        }
        roleCol = QStringLiteral("diastolic");
    }
        break;
    case Pulse:
    {
        if (bp.pulse() == value.toInt()) {
            return true;
        }
        roleCol = QStringLiteral("pulse");
    }
        break;
    case Note:
    {
        if (bp.note() == value.toString()) {
            return true;
        }
        roleCol = QStringLiteral("note");
    }
        break;
    }

    if (roleCol.isEmpty()) {
        qWarning() << "Invalid role" << role;
        return false;
    }

    QSqlQuery q;

    if (Q_UNLIKELY(!q.prepare(QStringLiteral("UPDATE bloodpressure SET %1 = :value WHERE id = :id").arg(roleCol)))) {
        // defined at peoplelistmodel.cpp
        setLastError(qtTrId("naz-err-failed-prepare-db-query").arg(q.lastError().text()));
        qCritical() << "Failed to prepare database query:" << q.lastError();
        return false;
    }

    q.bindValue(QStringLiteral(":value"), _value);
    q.bindValue(QStringLiteral(":id"), bp.id());

    if (Q_UNLIKELY(!q.exec())) {
        // defined at peoplelistmodel.cpp
        setLastError(qtTrId("naz-err-failed-execute-db-query").arg(q.lastError().text()));
        qCritical() << "Failed to execute database query:" << q.lastError();
        return false;
    }

    switch(role) {
    case Id:
    case PersonId:
    case RelDate:
        qWarning() << "Invalid role:" << role;
        return false;
    case Moment:
        bp.d->moment = value.toDateTime();
        break;
    case Systolic:
        bp.d->systolic = value.toInt();
        break;
    case Diastolic:
        bp.d->diastolic = value.toInt();
        break;
    case Pulse:
        bp.d->pulse = value.toInt();
        break;
    case Note:
        bp.d->note = value.toString();
        break;
    }

    m_bloodPressures[index.row()] = bp;

    QVector<int> changedRoles({role});
    if (role == Moment) {
        changedRoles.append(RelDate);
    }

    emit dataChanged(index, index, {changedRoles});

    return true;
}

int BloodPressureListModel::personId() const
{
    return m_personId;
}

void BloodPressureListModel::setPersonId(int id)
{
    if (m_personId != id) {
        qDebug() << "Changing personId from" << m_personId << "to" << id;
        m_personId = id;
        emit personIdChanged(personId());
    }
}

#include "moc_bloodpressurelistmodel.cpp"
