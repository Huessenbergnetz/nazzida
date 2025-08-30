/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "weightlistfiltermodel.h"
#include "weightlistmodel.h"

WeightListFilterModel::WeightListFilterModel(QObject *parent) : BaseFilterModel(parent)
{
    auto m = new WeightListModel(this);
    connect(m, &WeightListModel::personIdChanged, this, &WeightListFilterModel::personIdChanged);
    setModel(m);
}

WeightListFilterModel::~WeightListFilterModel()
{
}

bool WeightListFilterModel::loadForPerson(int personId)
{
    setPersonId(personId);
    return load();
}

bool WeightListFilterModel::add(const QDateTime &moment, int weight, const QString &note)
{
    auto m = qobject_cast<WeightListModel*>(model());
    return m ? m->add(moment, weight, note) : false;
}


bool WeightListFilterModel::remove(QModelIndex index)
{
    auto m = qobject_cast<WeightListModel*>(model());
    return m ? m->remove(mapToSource(index)) : false;
}

bool WeightListFilterModel::edit(QModelIndex index, const QDateTime &moment, int weight, const QString &note)
{
    auto m = qobject_cast<WeightListModel*>(model());
    return m ? m->edit(mapToSource(index), moment, weight, note) : false;
}

int WeightListFilterModel::personId() const
{
    auto m = qobject_cast<WeightListModel*>(model());
    return m ? m->personId() : 0;
}

void WeightListFilterModel::setPersonId(int id)
{
    auto m = qobject_cast<WeightListModel*>(model());
    if (m) {
        m->setPersonId(id);
    }
}

bool WeightListFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    auto l = sourceModel()->data(left, WeightListModel::Moment);
    auto r = sourceModel()->data(right, WeightListModel::Moment);

    auto lDt = sortRole() == Qt::DescendingOrder ? l.toDateTime() : r.toDateTime();
    auto rDt = sortRole() == Qt::DescendingOrder ? r.toDateTime() : l.toDateTime();

    return lDt < rDt;
}
