/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "liquidlistfiltermodel.h"
#include "liquidlistmodel.h"

LiquidListFilterModel::LiquidListFilterModel(QObject *parent) : BaseFilterModel(parent)
{
    auto m = new LiquidListModel(this);
    connect(m, &LiquidListModel::personIdChanged, this, &LiquidListFilterModel::personIdChanged);
    connect(m, &LiquidListModel::dayStartsChanged, this, &LiquidListFilterModel::dayStartsChanged);
    connect(m, &LiquidListModel::dayChanged, this, &LiquidListFilterModel::dayChanged);
    connect(m, &LiquidListModel::differenceChanged, this, &LiquidListFilterModel::differenceChanged);
    setModel(m);
}

LiquidListFilterModel::~LiquidListFilterModel()
{

}

bool LiquidListFilterModel::loadForPerson(int personId,  QTime dayStarts, QDate day)
{
    setPersonId(personId);
    setDayStarts(dayStarts);
    setDay(day);
    return load();
}

int LiquidListFilterModel::add(int id, const QDateTime &moment, int inOrOut, int amount, const QString &name, const QString &note)
{
    LiquidListModel* m = qobject_cast<LiquidListModel*>(model());
    return m ? m->add(id, moment, inOrOut, amount, name, note) : 0;
}

bool LiquidListFilterModel::remove(QModelIndex index)
{
    LiquidListModel* m = qobject_cast<LiquidListModel*>(model());
    return m ? m->remove(mapToSource(index)) : false;
}

bool LiquidListFilterModel::edit(QModelIndex index, const QDateTime &moment, int inOrOut, int amount, const QString &name, const QString &note)
{
    LiquidListModel* m = qobject_cast<LiquidListModel*>(model());
    return m ? m->edit(mapToSource(index), moment, inOrOut, amount, name, note) : false;
}

int LiquidListFilterModel::personId() const
{
    auto m = qobject_cast<LiquidListModel*>(model());
    return m ? m->personId() : 0;
}

void LiquidListFilterModel::setPersonId(int id)
{
    auto m = qobject_cast<LiquidListModel*>(model());
    if (m) {
        m->setPersonId(id);
    }
}

QTime LiquidListFilterModel::dayStarts() const
{
    auto m = qobject_cast<LiquidListModel*>(model());
    return m ? m->dayStarts() : QTime();
}

void LiquidListFilterModel::setDayStarts(QTime start)
{
    auto m = qobject_cast<LiquidListModel*>(model());
    if (m) {
        m->setDayStarts(start);
    }
}

QDate LiquidListFilterModel::day() const
{
    auto m = qobject_cast<LiquidListModel*>(model());
    return m ? m->day() : QDate();
}

void LiquidListFilterModel::setDay(QDate day)
{
    auto m = qobject_cast<LiquidListModel*>(model());
    if (m) {
        m->setDay(day);
    }
}

int LiquidListFilterModel::difference() const
{
    auto m = qobject_cast<LiquidListModel*>(model());
    return m ? m->difference() : 0;
}

bool LiquidListFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    auto l = sourceModel()->data(left, LiquidListModel::Moment);
    auto r = sourceModel()->data(right, LiquidListModel::Moment);

    auto lDt = sortRole() == Qt::DescendingOrder ? l.toDateTime() : r.toDateTime();
    auto rDt = sortRole() == Qt::DescendingOrder ? r.toDateTime() : l.toDateTime();

    return lDt < rDt;
}

#include "moc_liquidlistfiltermodel.cpp"
