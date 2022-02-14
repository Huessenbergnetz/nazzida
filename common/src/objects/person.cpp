/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "person.h"
#include <cmath>

Person::Person(QObject *parent) :
    QObject(parent)
{

}

Person::Person(int id, const QString &firstName, const QString &lastName, int size, QDate birthday, QTime dayStarts, const QString &sex, QObject *parent) :
    QObject(parent), m_firstName(firstName), m_lastName(lastName), m_sex(sex), m_birthday(birthday), m_dayStarts(dayStarts), m_id(id), m_size(size)
{

}

Person::~Person()
{

}

int Person::id() const
{
    return m_id;
}

QString Person::firstName() const
{
    return m_firstName;
}

void Person::setFirstName(const QString &name)
{
    if (m_firstName != name) {
        qDebug("Changing first name from \"%s\" to \"%s\"", qUtf8Printable(m_firstName), qUtf8Printable(name));
        m_firstName = name;
        emit firstNameChanged(firstName());
    }
}

QString Person::lastName() const
{
    return m_lastName;
}

void Person::setLastName(const QString &name)
{
    if (m_lastName != name) {
        qDebug("Changing last name from \"%s\" to \"%s\"", qUtf8Printable(m_lastName), qUtf8Printable(name));
        m_lastName = name;
        emit lastNameChanged(lastName());
    }
}

int Person::size() const
{
    return m_size;
}

void Person::setSize(int nSize)
{
    if (m_size != nSize) {
        qDebug("Changing size from %i to %i", m_size, nSize);
        m_size = nSize;
        emit sizeChanged(size());
    }
}

QDate Person::birthday() const
{
    return m_birthday;
}

void Person::setBirthday(QDate date)
{
    if (m_birthday != date) {
        qDebug("Changing birthday from \"%s\" to \"%s\"", qUtf8Printable(m_birthday.toString()), qUtf8Printable(date.toString()));
        m_birthday = date;
        emit birthdayChanged(birthday());
        emit ageChanged(age());
    }
}

QTime Person::dayStarts() const
{
    return m_dayStarts;
}

void Person::setDayStarts(QTime time)
{
    if (m_dayStarts != time) {
        qDebug("Changing dayStarts from \"%s\" to \"%s\"", qUtf8Printable(m_dayStarts.toString()), qUtf8Printable(time.toString()));
        m_dayStarts = time;
        emit dayStartsChanged(dayStarts());
    }
}

QString Person::sex() const
{
    return m_sex;
}

void Person::setSex(const QString &s)
{
    if (m_sex != s) {
        qDebug("Changing sex from \"%s\" to \"%s\"", qUtf8Printable(m_sex), qUtf8Printable(s));
        m_sex = s;
        emit sexChanged(sex());
    }
}

int Person::age() const
{
    const qint64 days = m_birthday.daysTo(QDate::currentDate());
    return std::abs(std::floor(days/365.25));
}

#include "moc_person.cpp"
