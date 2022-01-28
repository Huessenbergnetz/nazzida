/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef PEOPLELISTMODEL_H
#define PEOPLELISTMODEL_H

#include <QObject>
#include <vector>
#include "baselistmodel.h"

class Person;

class PeopleListModel : public BaseListModel
{
    Q_OBJECT
public:
    explicit PeopleListModel(QObject *parent = nullptr);
    ~PeopleListModel() override;

    enum Roles {
        Item = Qt::UserRole + 1,
        Id,
        FirstName,
        LastName,
        Size,
        Birthday,
        DayStarts,
        Sex,
        Age
    };
    Q_ENUM(Roles)

    Q_INVOKABLE bool load() override;
    Q_INVOKABLE void clear() override;
    Q_INVOKABLE int add(const QString &firstName, const QString &lastName, int size, const QDate &birthday, const QTime &dayStarts, const QString &sex);
    Q_INVOKABLE bool remove(const QModelIndex &index);
    Q_INVOKABLE bool edit(const QModelIndex &index, const QString &firstName, const QString &lastName, int size, const QDate &birthday, const QTime &dayStarts, const QString &sex);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

private:
    Q_DISABLE_COPY(PeopleListModel)
    std::vector<Person*> m_people;
};

#endif // PEOPLELISTMODEL_H
