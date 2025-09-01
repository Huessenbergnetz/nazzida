/*
 * SPDX-FileCopyrightText: (C) 2022-2025 Matthias Fehring / www.huessenbergnetz.de
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

    Q_INVOKABLE int add(const QString &firstName, const QString &lastName, int size, QDate birthday, QTime dayStarts, const QString &sex, int transpire);
    Q_INVOKABLE bool remove(QModelIndex index);
    Q_INVOKABLE bool edit(QModelIndex index, const QString &firstName, const QString &lastName, int size, QDate birthday, QTime dayStarts, const QString &sex, int transpire);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    Q_DISABLE_COPY(PeopleListFilterModel)
};

#endif // PEOPLELISTFILTERMODEL_H
