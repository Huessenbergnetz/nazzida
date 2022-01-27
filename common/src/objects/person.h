/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef PERSON_H
#define PERSON_H

#include <QObject>
#include <QDate>
#include <QTime>

class Person : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged)
    Q_PROPERTY(int size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QDate birthday READ birthday WRITE setBirthday NOTIFY birthdayChanged)
    Q_PROPERTY(QTime dayStarts READ dayStarts WRITE setDayStarts NOTIFY dayStartsChanged)
    Q_PROPERTY(QString sex READ sex WRITE setSex NOTIFY sexChanged)
public:
    explicit Person(QObject *parent = nullptr);
    Person(int id, const QString &firstName, const QString &lastName, int size, const QDate &birthday, const QTime &dayStarts, const QString &sex, QObject *parent = nullptr);
    ~Person() override;

    int id() const;

    QString firstName() const;
    void setFirstName(const QString &name);

    QString lastName() const;
    void setLastName(const QString &name);

    int size() const;
    void setSize(int nSize);

    QDate birthday() const;
    void setBirthday(const QDate &date);

    QTime dayStarts() const;
    void setDayStarts(const QTime &time);

    QString sex() const;
    void setSex(const QString &s);

signals:
    void firstNameChanged(const QString &firstName);
    void lastNameChanged(const QString &lastName);
    void sizeChanged(int size);
    void birthdayChanged(const QDate &birthday);
    void dayStartsChanged(const QTime &dayStarts);
    void sexChanged(const QString &sex);

private:
    QString m_firstName;
    QString m_lastName;
    QString m_sex;
    QDate m_birthday;
    QTime m_dayStarts;
    int m_id = 0;
    int m_size = 0;
};

#endif // PERSON_H
