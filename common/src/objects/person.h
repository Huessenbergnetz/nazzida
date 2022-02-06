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
    Q_PROPERTY(QString firstName READ firstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ lastName NOTIFY lastNameChanged)
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)
    Q_PROPERTY(QDate birthday READ birthday NOTIFY birthdayChanged)
    Q_PROPERTY(QTime dayStarts READ dayStarts NOTIFY dayStartsChanged)
    Q_PROPERTY(QString sex READ sex NOTIFY sexChanged)
    Q_PROPERTY(int age READ age NOTIFY ageChanged)
public:
    explicit Person(QObject *parent = nullptr);
    Person(int id, const QString &firstName, const QString &lastName, int size, QDate birthday, QTime dayStarts, const QString &sex, QObject *parent = nullptr);
    ~Person() override;

    int id() const;

    QString firstName() const;

    QString lastName() const;

    int size() const;

    QDate birthday() const;

    QTime dayStarts() const;

    QString sex() const;

    int age() const;

signals:
    void firstNameChanged(const QString &firstName);
    void lastNameChanged(const QString &lastName);
    void sizeChanged(int size);
    void birthdayChanged(const QDate &birthday);
    void dayStartsChanged(const QTime &dayStarts);
    void sexChanged(const QString &sex);
    void ageChanged(int age);

private:
    friend class PeopleListModel;

    void setFirstName(const QString &name);
    void setLastName(const QString &name);
    void setSize(int nSize);
    void setBirthday(QDate date);
    void setDayStarts(QTime time);
    void setSex(const QString &s);

    QString m_firstName;
    QString m_lastName;
    QString m_sex;
    QDate m_birthday;
    QTime m_dayStarts;
    int m_id = 0;
    int m_size = 0;
};

#endif // PERSON_H
