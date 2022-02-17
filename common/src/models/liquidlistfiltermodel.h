/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef LIQUIDLISTFILTERMODEL_H
#define LIQUIDLISTFILTERMODEL_H

#include "basefiltermodel.h"
#include <QObject>
#include <QDate>
#include <QTime>
#include "../objects/liquid.h"

class LiquidListFilterModel : public BaseFilterModel
{
    Q_OBJECT
    Q_PROPERTY(int personId READ personId WRITE setPersonId NOTIFY personIdChanged)
    Q_PROPERTY(QTime dayStarts READ dayStarts WRITE setDayStarts NOTIFY dayStartsChanged)
    Q_PROPERTY(QDate day READ day WRITE setDay NOTIFY dayChanged)
    Q_PROPERTY(int difference READ difference NOTIFY differenceChanged)
public:
    explicit LiquidListFilterModel(QObject *parent = nullptr);
   ~LiquidListFilterModel() override;

    Q_INVOKABLE bool loadForPerson(int personId,  QTime dayStarts, QDate day);

    Q_INVOKABLE int add(int id, const QDateTime &moment, int inOrOut, int amount, const QString &name, const QString &note);
    Q_INVOKABLE bool remove(QModelIndex index);
    Q_INVOKABLE bool edit(QModelIndex index, const QDateTime &moment, int inOrOut, int amount, const QString &name, const QString &note);

    int personId() const;
    void setPersonId(int id);

    QTime dayStarts() const;
    void setDayStarts(QTime start);

    QDate day() const;
    void setDay(QDate day);

    int difference() const;

signals:
    void personIdChanged(int personId);
    void dayStartsChanged(QTime dayStarts);
    void dayChanged(QDate day);
    void differenceChanged(int difference);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};

#endif // LIQUIDLISTFILTERMODEL_H
