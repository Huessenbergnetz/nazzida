/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "peoplelistmodel.h"
#include "../objects/person.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QTime>

PeopleListModel::PeopleListModel(QObject *parent) :
    BaseListModel(parent)
{

}

PeopleListModel::~PeopleListModel()
{
    qDeleteAll(m_people);
    m_people.clear();
}

bool PeopleListModel::load()
{
    setInOperation(true);

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

    if (!q.exec(QStringLiteral("PRAGMA foreign_keys = ON"))) {
        setLastError(qtTrId("naz-err-failed-foreign-keys-pragma").arg(q.lastError().text()));
        qCritical("Failed to enable foreign keys pragma: %s", qUtf8Printable(q.lastError().text()));
        setInOperation(false);
        return false;
    }

    if (!q.exec(QStringLiteral("SELECT id, first_name, last_name, size, birthday, day_starts, sex FROM people"))) {
        //: error message, %1 will be replaced by the database error message
        //% "Failed to execute database query: %1"
        setLastError(qtTrId("naz-err-failed-execute-db-query").arg(q.lastError().text()));
        qCritical("Failed to execute database query: %s", qUtf8Printable(q.lastError().text()));
        setInOperation(false);
        return false;
    }

    std::vector<Person*> _people;

    while(q.next()) {
        _people.push_back(new Person(q.value(0).toInt(),
                                     q.value(1).toString(),
                                     q.value(2).toString(),
                                     q.value(3).toInt(),
                                     q.value(4).toDate(),
                                     q.value(5).toTime(),
                                     q.value(6).toString(),
                                     this));
    }

    if (!_people.empty()) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount() + _people.size() - 1);
        m_people = _people;
        endInsertRows();
    }

    setInOperation(false);
    return true;
}

void PeopleListModel::clear()
{
    if (!m_people.empty()) {
        setInOperation(true);
        beginRemoveRows(QModelIndex(), 0, m_people.size() - 1);
        qDeleteAll(m_people);
        m_people.clear();
        endRemoveRows();
        setInOperation(false);
    }
}

int PeopleListModel::add(const QString &firstName, const QString &lastName, int size, const QDate &birthday, const QTime &dayStarts, const QString &sex)
{
    QSqlQuery q;

    if (!q.exec(QStringLiteral("PRAGMA foreign_keys = ON"))) {
        setLastError(qtTrId("naz-err-failed-foreign-keys-pragma").arg(q.lastError().text()));
        qCritical("Failed to enable foreign keys pragma: %s", qUtf8Printable(q.lastError().text()));
        return 0;
    }

    if (!q.prepare(QStringLiteral("INSERT INTO people (first_name, last_name, size, birthday, day_starts, sex) VALUES (:first_name, :last_name, :size, :birthday, :day_starts, :sex)"))) {
        //: error message, %1 will be replaced by the database error message
        //% "Failed to prepare database query: %1"
        setLastError(qtTrId("naz-err-failed-prepare-db-query").arg(q.lastError().text()));
        qCritical("Failed to prepare database query: %s", qUtf8Printable(q.lastError().text()));
        return 0;
    }
    q.bindValue(QStringLiteral(":first_name"), firstName);
    q.bindValue(QStringLiteral(":last_name"), lastName);
    q.bindValue(QStringLiteral(":size"), size);
    q.bindValue(QStringLiteral(":birthday"), birthday);
    q.bindValue(QStringLiteral(":day_starts"), dayStarts);
    q.bindValue(QStringLiteral(":sex"), sex);

    if (!q.exec()) {
        setLastError(qtTrId("naz-err-failed-execute-db-query").arg(q.lastError().text()));
        qCritical("Failed to execute database query: %s", qUtf8Printable(q.lastError().text()));
        return 0;
    }

    const int id = q.lastInsertId().toInt();

    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    m_people.push_back(new Person(id,
                                  firstName,
                                  lastName,
                                  size,
                                  birthday,
                                  dayStarts,
                                  sex,
                                  this));

    endInsertRows();

    qDebug("Created new Person %s %s with ID %i, born at %s of size %i. Day starts at %s.", qUtf8Printable(firstName), qUtf8Printable(lastName), id, qUtf8Printable(birthday.toString()), size, qUtf8Printable(dayStarts.toString()));

    return id;
}

QHash<int, QByteArray> PeopleListModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(Item, QByteArrayLiteral("item"));
    roles.insert(Id, QByteArrayLiteral("id"));
    roles.insert(FirstName, QByteArrayLiteral("firstName"));
    roles.insert(LastName, QByteArrayLiteral("lastName"));
    roles.insert(Size, QByteArrayLiteral("size"));
    roles.insert(Birthday, QByteArrayLiteral("birthday"));
    roles.insert(DayStarts, QByteArrayLiteral("dayStarts"));
    roles.insert(Sex, QByteArrayLiteral("sex"));
    roles.insert(Age, QByteArrayLiteral("age"));
    return roles;
}

int PeopleListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_people.size();
}

QModelIndex PeopleListModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}

QVariant PeopleListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() >= rowCount())) {
        return QVariant();
    }

    Person *p = m_people.at(index.row());

    switch(role) {
    case Item:
        return QVariant::fromValue<Person*>(p);
    case Id:
        return QVariant::fromValue<int>(p->id());
    case FirstName:
        return QVariant::fromValue<QString>(p->firstName());
    case LastName:
        return QVariant::fromValue<QString>(p->lastName());
    case Size:
        return QVariant::fromValue<int>(p->size());
    case Birthday:
        return QVariant::fromValue<QDate>(p->birthday());
    case DayStarts:
        return QVariant::fromValue<QTime>(p->dayStarts());
    case Sex:
        return QVariant::fromValue<QString>(p->sex());
    case Age:
        return QVariant::fromValue<int>(p->age());
    default:
        return QVariant();
    }
}

bool PeopleListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || (index.row() >= rowCount())) {
        qWarning("%s", "Invalid index");
        return false;
    }

    Person *p = m_people.at(index.row());

    QString roleColumn;
    switch(role) {
    case FirstName:
    {
        if (p->firstName() == value.toString()) {
            return true;
        }
        roleColumn = QStringLiteral("first_name");
    }
        break;
    case LastName:
    {
        if (p->lastName() == value.toString()) {
            return true;
        }
        roleColumn = QStringLiteral("last_name");
    }
        break;
    case Size:
    {
        if (p->size() == value.toInt()) {
            return true;
        }
        roleColumn = QStringLiteral("size");
    }
        break;
    case Birthday:
    {
        if (p->birthday() == value.toDate()) {
            return true;
        }
        roleColumn = QStringLiteral("birthday");
    }
        break;
    case DayStarts:
    {
        if (p->dayStarts() == value.toTime()) {
            return true;
        }
        roleColumn = QStringLiteral("day_starts");
    }
        break;
    case Sex:
    {
        if (p->sex() == value.toString()) {
            return true;
        }
        roleColumn = QStringLiteral("sex");
    }
        break;
    default:
        return false;
    }

    QSqlQuery q;
    if (!q.prepare(QStringLiteral("UPDATE people SET %1 = :value WHERE id = :id").arg(roleColumn))) {
        setLastError(qtTrId("naz-err-failed-prepare-db-query").arg(q.lastError().text()));
        qCritical("Failed to prepare database query: %s", qUtf8Printable(q.lastError().text()));
        return false;
    }
    q.bindValue(QStringLiteral(":value"), value);
    q.bindValue(QStringLiteral(":id"), p->id());

    if (!q.exec()) {
        setLastError(qtTrId("naz-err-failed-execute-db-query").arg(q.lastError().text()));
        qCritical("Failed to execute database query: %s", qUtf8Printable(q.lastError().text()));
        return false;
    }

    switch(role) {
    case FirstName:
        p->setFirstName(value.toString());
        break;
    case LastName:
        p->setLastName(value.toString());
        break;
    case Size:
        p->setSize(value.toInt());
        break;
    case Birthday:
        p->setBirthday(value.toDate());
        break;
    case DayStarts:
        p->setDayStarts(value.toTime());
        break;
    case Sex:
        p->setSex(value.toString());
        break;
    default:
        break;
    }

    dataChanged(index, index, {role});

    return true;
}

bool PeopleListModel::remove(const QModelIndex &index)
{
    if (!index.isValid() || (index.row() >= rowCount())) {
        qWarning("%s", "Invalid index");
        return false;
    }

    Person *p = m_people.at(index.row());

    QSqlQuery q;

    if (!q.exec(QStringLiteral("PRAGMA foreign_keys = ON"))) {
        setLastError(qtTrId("naz-err-failed-foreign-keys-pragma").arg(q.lastError().text()));
        qCritical("Failed to enable foreign keys pragma: %s", qUtf8Printable(q.lastError().text()));
        return false;
    }

    if (!q.prepare(QStringLiteral("DELETE FROM people WHERE id = :id"))) {
        setLastError(qtTrId("naz-err-failed-prepare-db-query").arg(q.lastError().text()));
        qCritical("Failed to prepare database query: %s", qUtf8Printable(q.lastError().text()));
        return false;
    }

    q.bindValue(QStringLiteral(":id"), p->id());

    if (!q.exec()) {
        setLastError(qtTrId("naz-err-failed-execute-db-query").arg(q.lastError().text()));
        qCritical("Failed to execute database query: %s", qUtf8Printable(q.lastError().text()));
        return false;
    }

    beginRemoveRows(QModelIndex(), index.row(), index.row());

    m_people.erase(m_people.begin() + index.row());

    endRemoveRows();

    qDebug("Removed Person %s %s with ID %i", qUtf8Printable(p->firstName()),qUtf8Printable(p->lastName()), p->id());
    delete p;

    return true;
}

bool PeopleListModel::edit(const QModelIndex &index, const QString &firstName, const QString &lastName, int size, const QDate &birthday, const QTime &dayStarts, const QString &sex)
{
    if (!index.isValid() || (index.row() >= rowCount())) {
        qWarning("%s", "Invalid index");
        return false;
    }

    Person *p = m_people.at(index.row());

    if (!setData(index, firstName, FirstName)) {
        return false;
    }
    p->setFirstName(firstName);

    if (!setData(index, lastName, LastName)) {
        return false;
    }
    p->setLastName(lastName);

    if (!setData(index, size, Size)) {
        return false;
    }
    p->setSize(size);

    if (!setData(index, birthday, Birthday)) {
        return false;
    }
    p->setBirthday(birthday);

    if (!setData(index, dayStarts, DayStarts)) {
        return false;
    }
    p->setDayStarts(dayStarts);

    if (!setData(index, sex, Sex)) {
        return false;
    }
    p->setSex(sex);

    return true;
}

#include "moc_peoplelistmodel.cpp"
