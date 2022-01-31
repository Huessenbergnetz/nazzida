/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "basefiltermodel.h"
#include "baselistmodel.h"
#include <QDebug>

BaseFilterModel::BaseFilterModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

BaseFilterModel::~BaseFilterModel()
{

}

bool BaseFilterModel::load()
{
    if (m_model) {
        if (m_model->load()) {
            sort(0);
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
    return m_model ? m_model->load() : false;
}

void BaseFilterModel::clear()
{
    if (m_model) {
        m_model->clear();
    }
}

bool BaseFilterModel::inOperation() const
{
    return m_model ? m_model->inOperation() : false;
}

QString BaseFilterModel::lastError() const
{
    return m_model ? m_model->lastError() : QString();
}

void BaseFilterModel::setModel(BaseListModel *model)
{
    m_model = model;
    connect(m_model, &BaseListModel::inOperationChanged, this, &BaseFilterModel::inOperationChanged);
    connect(m_model, &BaseListModel::lastErrorChanged, this, &BaseFilterModel::lastErrorChanged);
    setSourceModel(m_model);
}

BaseListModel* BaseFilterModel::model() const
{
    return m_model;
}

Qt::SortOrder BaseFilterModel::sortOrder() const
{
    return m_sortOrder;
}

void BaseFilterModel::setSortOrder(Qt::SortOrder order)
{
    if (m_sortOrder != order) {
        qDebug() << "Changing sortOrder from" << m_sortOrder << "to" << order;
        m_sortOrder = order;
        emit sortOrderChanged(sortOrder());
        sort(0, m_sortOrder);
    }
}

#include "moc_basefiltermodel.cpp"
