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
    setModel(m);
}

LiquidListFilterModel::~LiquidListFilterModel()
{

}

bool LiquidListFilterModel::loadForPerson(int personId)
{
    setPersonId(personId);
    return load();
}

int LiquidListFilterModel::add(const QDateTime &moment, int inOrOut, int amount, const QString &name, const QString &note)
{
    LiquidListModel* m = qobject_cast<LiquidListModel*>(model());
    return m ? m->add(moment, inOrOut, amount, name, note) : 0;
}

bool LiquidListFilterModel::remove(const QModelIndex &index)
{
    LiquidListModel* m = qobject_cast<LiquidListModel*>(model());
    return m ? m->remove(mapToSource(index)) : false;
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

bool LiquidListFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    auto l = sourceModel()->data(left, LiquidListModel::Moment);
    auto r = sourceModel()->data(right, LiquidListModel::Moment);

    auto lDt = sortRole() == Qt::AscendingOrder ? l.toDateTime() : r.toDateTime();
    auto rDt = sortRole() == Qt::DescendingOrder ? r.toDateTime() : l.toDateTime();

    return lDt < rDt;
}

#include "moc_liquidlistfiltermodel.cpp"
