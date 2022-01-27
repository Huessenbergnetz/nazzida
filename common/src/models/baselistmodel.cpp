/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "baselistmodel.h"

BaseListModel::BaseListModel(QObject *parent) : QAbstractListModel(parent)
{

}

BaseListModel::~BaseListModel()
{

}

bool BaseListModel::load()
{
    return false;
}

void BaseListModel::clear()
{

}

bool BaseListModel::inOperation() const
{
    return m_inOperation;
}

void BaseListModel::setInOperation(bool inOp)
{
    if (m_inOperation != inOp) {
        qDebug("Changing inOperation from %s to %s", m_inOperation ? "true" : "false", inOp ? "true" : "false");
        m_inOperation = inOp;
        emit inOperationChanged(inOperation());
    }
}

QString BaseListModel::lastError() const
{
    return m_lastError;
}

void BaseListModel::setLastError(const QString &error)
{
    if (m_lastError != error) {
        qDebug("Changing lastError from \"%s\" to \"%s\"", qUtf8Printable(m_lastError), qUtf8Printable(error));
        m_lastError = error;
        emit lastErrorChanged(lastError());
    }
}

#include "moc_baselistmodel.cpp"
