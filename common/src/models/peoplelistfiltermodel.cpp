/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "peoplelistfiltermodel.h"
#include "peoplelistmodel.h"
#include "../objects/person.h"

PeopleListFilterModel::PeopleListFilterModel(QObject *parent) : BaseFilterModel(parent)
{
    setModel(new PeopleListModel(this));
}

PeopleListFilterModel::~PeopleListFilterModel()
{

}

int PeopleListFilterModel::add(const QString &firstName, const QString &lastName, int size, const QDate &birthday, const QTime &dayStarts, const QString &sex)
{
    if (model()) {
        PeopleListModel *m = qobject_cast<PeopleListModel*>(model());
        if (m) {
            return m->add(firstName, lastName, size, birthday, dayStarts, sex);
        } else {
            return false;
        }
    } else {
        return 0;
    }
}

bool PeopleListFilterModel::remove(const QModelIndex &index)
{
    if (model()) {
        PeopleListModel *m = qobject_cast<PeopleListModel*>(model());
        if (m) {
            return m->remove(mapToSource(index));
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool PeopleListFilterModel::edit(const QModelIndex &index, const QString &firstName, const QString &lastName, int size, const QDate &birthday, const QTime &dayStarts, const QString &sex)
{
    if (model()) {
        PeopleListModel *m = qobject_cast<PeopleListModel*>(model());
        if (m) {
            return m->edit(mapToSource(index), firstName, lastName, size, birthday, dayStarts, sex);
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool PeopleListFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    Person *l = sourceModel()->data(left, PeopleListModel::Item).value<Person*>();
    Person *r = sourceModel()->data(right, PeopleListModel::Item).value<Person*>();
    Person *lP = l;
    Person *rP = r;

    if (sortOrder() == Qt::DescendingOrder) {
        lP = r;
        rP = l;
    }

    return lP->firstName().localeAwareCompare(rP->firstName()) <= 0;
}

#include "moc_peoplelistfiltermodel.cpp"
