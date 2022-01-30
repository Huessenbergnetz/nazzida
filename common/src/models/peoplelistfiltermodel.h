/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef PEOPLELISTFILTERMODEL_H
#define PEOPLELISTFILTERMODEL_H

#include "basefiltermodel.h"
#include <QObject>

class PeopleListFilterModel : public BaseFilterModel
{
    Q_OBJECT
public:
    explicit PeopleListFilterModel(QObject *parent = nullptr);
    ~PeopleListFilterModel() override;

    Q_INVOKABLE int add(const QString &firstName, const QString &lastName, int size, const QDate &birthday, const QTime &dayStarts, const QString &sex);
    Q_INVOKABLE bool remove(const QModelIndex &index);
    Q_INVOKABLE bool edit(const QModelIndex &index, const QString &firstName, const QString &lastName, int size, const QDate &birthday, const QTime &dayStarts, const QString &sex);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};

#endif // PEOPLELISTFILTERMODEL_H
