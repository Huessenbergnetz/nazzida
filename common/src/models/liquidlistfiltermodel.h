/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef LIQUIDLISTFILTERMODEL_H
#define LIQUIDLISTFILTERMODEL_H

#include "basefiltermodel.h"
#include <QObject>
#include "../objects/liquid.h"

class LiquidListFilterModel : public BaseFilterModel
{
    Q_OBJECT
    Q_PROPERTY(int personId READ personId WRITE setPersonId NOTIFY personIdChanged)
public:
    explicit LiquidListFilterModel(QObject *parent = nullptr);
   ~LiquidListFilterModel() override;

    Q_INVOKABLE bool loadForPerson(int personId);

    Q_INVOKABLE int add(const QDateTime &moment, int inOrOut, int amount, const QString &name, const QString &note);
    Q_INVOKABLE bool remove(QModelIndex index);

    int personId() const;
    void setPersonId(int id);

signals:
    void personIdChanged(int personId);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};

#endif // LIQUIDLISTFILTERMODEL_H
