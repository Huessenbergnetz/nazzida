/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "bloodpressurelistfiltermodel.h"
#include "bloodpressurelistmodel.h"
#include "../objects/bloodpressure.h"

BloodPressureListFilterModel::BloodPressureListFilterModel(QObject *parent)
    : BaseFilterModel(parent)
{
    auto m = new BloodPressureListModel(this);
    connect(m, &BloodPressureListModel::personIdChanged, this, &BloodPressureListFilterModel::personIdChanged);
    setModel(m);
}

BloodPressureListFilterModel::~BloodPressureListFilterModel() = default;

bool BloodPressureListFilterModel::loadForPerson(int personId)
{
    setPersonId(personId);
    return load();
}

bool BloodPressureListFilterModel::add(const QDateTime &moment, int systolic, int diastolic, int pulse, const QString &note)
{
    auto m = qobject_cast<BloodPressureListModel*>(model());
    return m ? m->add(moment, systolic, diastolic, pulse, note) : false;
}

bool BloodPressureListFilterModel::remove(QModelIndex index)
{
    auto m = qobject_cast<BloodPressureListModel*>(model());
    return m ? m->remove(mapToSource(index)) : false;
}

bool BloodPressureListFilterModel::edit(QModelIndex index, const QDateTime &moment, int systolic, int diastolic, int pulse, const QString &note)
{
    auto m = qobject_cast<BloodPressureListModel*>(model());
    return m ? m->edit(mapToSource(index), moment, systolic, diastolic, pulse, note) : false;
}

int BloodPressureListFilterModel::personId() const
{
    auto m = qobject_cast<BloodPressureListModel*>(model());
    return m ? m->personId() : 0;
}

void BloodPressureListFilterModel::setPersonId(int id)
{
    auto m = qobject_cast<BloodPressureListModel*>(model());
    if (m) {
        m->setPersonId(id);
    }
}

bool BloodPressureListFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    auto l = sourceModel()->data(left, BloodPressureListModel::Moment);
    auto r = sourceModel()->data(right, BloodPressureListModel::Moment);

    auto lDt = sortRole() == Qt::DescendingOrder ? l.toDateTime() : r.toDateTime();
    auto rDt = sortRole() == Qt::DescendingOrder ? r.toDateTime() : l.toDateTime();

    return lDt < rDt;
}

#include "moc_bloodpressurelistfiltermodel.cpp"
