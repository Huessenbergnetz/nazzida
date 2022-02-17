/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "dailyliquidlistfiltermodel.h"
#include "dailyliquidlistmodel.h"

DailyLiquidListFilterModel::DailyLiquidListFilterModel(QObject *parent) : BaseFilterModel(parent)
{
    auto m = new DailyLiquidListModel(this);
    connect(m, &DailyLiquidListModel::personIdChanged, this, &DailyLiquidListFilterModel::personIdChanged);
    connect(m, &DailyLiquidListModel::dayStartsChanged, this, &DailyLiquidListFilterModel::dayStartsChanged);
    setModel(m);
}

DailyLiquidListFilterModel::~DailyLiquidListFilterModel()
{

}

bool DailyLiquidListFilterModel::loadForPerson(int personId, QTime dayStarts)
{
    setPersonId(personId);
    setDayStarts(dayStarts);
    return load();
}

int DailyLiquidListFilterModel::add(const QDateTime &moment, int inOrOut, int amount, const QString &name, const QString &note)
{
    DailyLiquidListModel* m = qobject_cast<DailyLiquidListModel*>(model());
    return m ? m->add(moment, inOrOut, amount, name, note) : 0;
}

int DailyLiquidListFilterModel::personId() const
{
    DailyLiquidListModel* m = qobject_cast<DailyLiquidListModel*>(model());
    return m ? m->personId() : 0;
}

void DailyLiquidListFilterModel::setPersonId(int id)
{
    DailyLiquidListModel* m = qobject_cast<DailyLiquidListModel*>(model());
    if (m) {
        m->setPersonId(id);
    }
}

QTime DailyLiquidListFilterModel::dayStarts() const
{
    DailyLiquidListModel* m = qobject_cast<DailyLiquidListModel*>(model());
    return m ? m->dayStarts() : QTime();
}

void DailyLiquidListFilterModel::setDayStarts(QTime dayStarts)
{
    DailyLiquidListModel* m = qobject_cast<DailyLiquidListModel*>(model());
    if (m) {
        m->setDayStarts(dayStarts);
    }
}

bool DailyLiquidListFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    auto l = sourceModel()->data(left, DailyLiquidListModel::Day);
    auto r = sourceModel()->data(right, DailyLiquidListModel::Day);

    auto lDt = sortRole() == Qt::DescendingOrder ? l.toDate() : r.toDate();
    auto rDt = sortRole() == Qt::DescendingOrder ? r.toDate() : l.toDate();

    return lDt < rDt;
}

#include "moc_dailyliquidlistfiltermodel.cpp"
