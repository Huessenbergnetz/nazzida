/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef DAILYLIQUIDLISTFILTERMODEL_H
#define DAILYLIQUIDLISTFILTERMODEL_H

#include "basefiltermodel.h"
#include <QObject>
#include <QTime>

class DailyLiquidListFilterModel : public BaseFilterModel
{
    Q_OBJECT
    Q_PROPERTY(int personId READ personId WRITE setPersonId NOTIFY personIdChanged)
    Q_PROPERTY(QTime dayStarts READ dayStarts WRITE setDayStarts NOTIFY dayStartsChanged)
public:
    explicit DailyLiquidListFilterModel(QObject *parent = nullptr);
    ~DailyLiquidListFilterModel() override;

    Q_INVOKABLE bool loadForPerson(int personId, QTime dayStarts);

    Q_INVOKABLE int add(const QDateTime &moment, int inOrOut, int amount, const QString &name, const QString &note);
    Q_INVOKABLE bool remove(const QDateTime &moment, int inOrOut, int amount);

    int personId() const;
    void setPersonId(int id);

    QTime dayStarts() const;
    void setDayStarts(QTime dayStarts);

signals:
    void personIdChanged(int personId);
    void dayStartsChanged(QTime dayStarts);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};

#endif // DAILYLIQUIDLISTFILTERMODEL_H
