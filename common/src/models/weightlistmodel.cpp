/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "weightlistmodel.h"
#include "../objects/weight_p.h"

#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

WeightListModel::WeightListModel(QObject *parent)
    : BaseListModel(parent)
{
}

WeightListModel::~WeightListModel()
{
}

bool WeightListModel::load()
{
    setInOperation(true);

    qDebug("Loading weights for person ID %i", personId());

    {
        auto db = QSqlDatabase::database();
        if (Q_UNLIKELY(!db.open())) {
            setLastError(qtTrId("naz-err-failed-open-db").arg(db.databaseName(), db.lastError().text()));
            qCritical("Failed to open database %s: %s", qUtf8Printable(db.databaseName()), qUtf8Printable(db.lastError().text()));
            setInOperation(false);
            return false;
        }
    }

    clear();

    QSqlQuery q;

    if (Q_UNLIKELY(!q.prepare(QStringLiteral("SELECT id, person_id, moment, weight, note FROM weight WHERE person_id = :person_id ORDER BY moment DESC")))) {
        setLastError(qtTrId("naz-err-failed-prepare-db-query").arg(q.lastError().text()));
        qCritical("Failed to prepare database query: %s", qUtf8Printable(q.lastError().text()));
        setInOperation(false);
        return false;
    }

    q.bindValue(QStringLiteral(":person_id"), personId());

    if (Q_UNLIKELY(!q.exec())) {
        setLastError(qtTrId("naz-err-failed-execute-db-query").arg(q.lastError().text()));
        qCritical("Failed to execute database query: %s", qUtf8Printable(q.lastError().text()));
        setInOperation(false);
        return false;
    }

    std::vector<Weight> _weights;
    while (q.next()) {
        auto momentUtc = q.value(2).toDateTime();
        momentUtc.setTimeSpec(Qt::UTC);

        _weights.emplace_back(q.value(0).toInt(),
                              q.value(1).toInt(),
                              momentUtc.toLocalTime(),
                              q.value(3).toInt(),
                              q.value(4).toString());
    }

    if (!_weights.empty()) {
        beginInsertRows(QModelIndex(), 0, _weights.size() - 1);
        m_weights = _weights;
        endInsertRows();
    }

    setInOperation(false);

    return true;
}

bool WeightListModel::loadForPerson(int personId)
{
    setPersonId(personId);
    return load();
}

void WeightListModel::clear()
{
    if (!m_weights.empty()) {
        beginRemoveRows(QModelIndex(), 0, m_weights.size() - 1);
        m_weights.clear();
        endRemoveRows();
    }
}

bool WeightListModel::add(const QDateTime &moment, int weight, const QString &note)
{
    QSqlQuery q;

    if (Q_UNLIKELY(!q.exec(QStringLiteral("PRAGMA foreign_keys = ON")))) {
        setLastError(qtTrId("naz-err-failed-foreign-keys-pragma").arg(q.lastError().text()));
        qCritical("Failed to enable foreign keys pragma: %s", qUtf8Printable(q.lastError().text()));
        return false;
    }

    if (Q_UNLIKELY(!q.prepare(QStringLiteral("INSERT INTO weight (person_id, moment, weight, note) VALUES (:person_id, :moment, :weight, :note)")))) {
        setLastError(qtTrId("naz-err-failed-prepare-db-query").arg(q.lastError().text()));
        qCritical("Failed to prepare database query %s", qUtf8Printable(q.lastError().text()));
        return false;
    }

    q.bindValue(QStringLiteral(":person_id"), personId());
    q.bindValue(QStringLiteral(":moment"), moment.toUTC());
    q.bindValue(QStringLiteral(":weight"), weight);
    q.bindValue(QStringLiteral(":note"), note);

    if (Q_UNLIKELY(!q.exec())) {
        setLastError(qtTrId("naz-err-failed-execute-db-query").arg(q.lastError().text()));
        qCritical("Failed to execute database query %s", qUtf8Printable(q.lastError().text()));
        return false;
    }

    const int id = q.lastInsertId().toInt();

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_weights.emplace_back(id, personId(), moment, weight, note);
    endInsertRows();

    qDebug("Created new weight %i g with ID %i for person ID %i", weight, id, personId());

    return true;
}

bool WeightListModel::remove(QModelIndex index)
{
    if (!index.isValid() || (index.row() >= rowCount())) {
        qWarning() << "Invalid" << index;
        return false;
    }

    const Weight w = m_weights.at(index.row());

    QSqlQuery q;

    if (Q_UNLIKELY(!q.prepare(QStringLiteral("DELETE FROM weight WHERE id = :id")))) {
        setLastError(qtTrId("naz-err-failed-prepare-db-query").arg(q.lastError().text()));
        qCritical("Failed to prepare database query: %s", qUtf8Printable(q.lastError().text()));
        return false;
    }

    q.bindValue(QStringLiteral(":id"), w.id());

    if (Q_UNLIKELY(!q.exec())) {
        setLastError(qtTrId("naz-err-failed-execute-db-query").arg(q.lastError().text()));
        qCritical("Failed to execute database query: %s", qUtf8Printable(q.lastError().text()));
        return false;
    }

    beginRemoveRows(QModelIndex(), index.row(), index.row());

    m_weights.erase(m_weights.begin() + index.row());

    endRemoveRows();

    qDebug("Removed weight %i g with ID %i from person ID %i", w.weight(), w.id(), w.personId());

    return true;
}

bool WeightListModel::edit(QModelIndex index, const QDateTime &moment, int weight, const QString &note)
{
    if (!index.isValid() || index.row() >= rowCount()) {
        qWarning("Invalid index: %i", index.row());
        return false;
    }

    if (!setData(index, moment, Moment)) {
        return false;
    }

    if (!setData(index, weight, WeightValue)) {
        return false;
    }

    if (!setData(index, note, Note)) {
        return false;
    }

    return true;
}

QModelIndex WeightListModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}

QVariant WeightListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() >= rowCount())) {
        qWarning("Invalid index: %i", index.row());
        return QVariant();
    }

    const auto w = m_weights.at(index.row());

    switch(role) {
    case Id:
        return w.id();
    case PersonId:
        return w.personId();
    case Moment:
        return w.moment();
    case WeightValue:
        return w.weight();
    case Note:
        return w.note();
    }

    return QVariant();
}

int WeightListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_weights.size();
}

QHash<int, QByteArray> WeightListModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(Id, QByteArrayLiteral("id"));
    roles.insert(PersonId, QByteArrayLiteral("personId"));
    roles.insert(Moment, QByteArrayLiteral("moment"));
    roles.insert(WeightValue, QByteArrayLiteral("weight"));
    roles.insert(Note, QByteArrayLiteral("note"));
    return roles;
}

bool WeightListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || (index.row() >= rowCount())) {
        qWarning("Invalid index: %i", index.row());
        return false;
    }

    Weight w = m_weights.at(index.row());

    QString roleCol;
    QVariant _value = value;

    switch(role) {
    case Id:
    case PersonId:
    {
        qWarning("Can not change id or personId");
        return false;
    }
    case Moment:
    {
        if (w.moment() == value.toDateTime()) {
            return true;
        }
        roleCol = QStringLiteral("moment");
        _value = value.toDateTime().toUTC();
    }
        break;
    case WeightValue:
    {
        if (w.weight() == value.toInt()) {
            return true;
        }
        roleCol = QStringLiteral("weight");
    }
        break;
    case Note:
    {
        if (w.note() == value.toString()) {
            return true;
        }
        roleCol = QStringLiteral("note");
    }
        break;
    }

    if (roleCol.isEmpty()) {
        qWarning("Invalid role");
        return false;
    }

    QSqlQuery q;

    if (Q_UNLIKELY(!q.prepare(QStringLiteral("UPDATE weight SET %1 = :value WHERE id = :id").arg(roleCol)))) {
        // defined at peoplelistmodel.cpp
        setLastError(qtTrId("naz-err-failed-prepare-db-query").arg(q.lastError().text()));
        qCritical("Failed to prepare database query: %s", qUtf8Printable(q.lastError().text()));
        return false;
    }

    q.bindValue(QStringLiteral(":value"), _value);
    q.bindValue(QStringLiteral(":id"), w.id());

    if (Q_UNLIKELY(!q.exec())) {
        // defined at peoplelistmodel.cpp
        setLastError(qtTrId("naz-err-failed-execute-db-query").arg(q.lastError().text()));
        qCritical("Failed to execute database query: %s", qUtf8Printable(q.lastError().text()));
        return false;
    }

    switch(role) {
    case Moment:
        w.d->moment = value.toDateTime();
        break;
    case WeightValue:
        w.d->weight = value.toInt();
        break;
    case Note:
        w.d->note = value.toString();
        break;
    case Id:
    case PersonId:
        qWarning("Invalid role");
        return false;
    }

    m_weights[index.row()] = w;

    emit dataChanged(index, index, {role});

    return true;
}

int WeightListModel::personId() const
{
    return m_personId;
}

void WeightListModel::setPersonId(int id)
{
    if (m_personId != id) {
        qDebug("Changing personId from %i to %i", m_personId, id);
        m_personId = id;
        emit personIdChanged(personId());
    }
}

#include "moc_weightlistmodel.cpp"
