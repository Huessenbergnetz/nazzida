/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef BLOODPRESSURELISTFILTERMODEL_H
#define BLOODPRESSURELISTFILTERMODEL_H

#include "basefiltermodel.h"

#include <QObject>

class BloodPressureListFilterModel : public BaseFilterModel
{
    Q_OBJECT
    Q_PROPERTY(int personId READ personId WRITE setPersonId NOTIFY personIdChanged)
public:
    explicit BloodPressureListFilterModel(QObject *parent = nullptr);
    ~BloodPressureListFilterModel() override;

    Q_INVOKABLE bool loadForPerson(int personId);

    Q_INVOKABLE bool add(const QDateTime &moment, int systolic, int diastolic, int pulse, const QString &note);
    Q_INVOKABLE bool remove(QModelIndex index);
    Q_INVOKABLE bool edit(QModelIndex index, const QDateTime &moment, int systolic, int diastolic, int pulse, const QString &note);

    int personId() const;
    void setPersonId(int id);

signals:
    void personIdChanged(int personId);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    Q_DISABLE_COPY(BloodPressureListFilterModel)
};

#endif // BLOODPRESSURELISTFILTERMODEL_H
