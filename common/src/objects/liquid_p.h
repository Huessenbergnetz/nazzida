/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef LIQUID_P_H
#define LIQUID_P_H

#include "liquid.h"
#include <QSharedData>

class LiquidData : public QSharedData
{
public:
    LiquidData() : QSharedData() {}

    LiquidData(int _id, int _personId, const QDateTime &_moment, Liquid::InOrOut _inOrOut, int _amount, const QString &_name, const QString &_note) :
        QSharedData(),
        name(_name),
        note(_note),
        moment(_moment),
        id(_id),
        personId(_personId),
        amount(_amount),
        inOrOut(_inOrOut)
    {}

    ~LiquidData() {}

    QString name;
    QString note;
    QDateTime moment;
    int id = 0;
    int personId = 0;
    int amount = 0;
    Liquid::InOrOut inOrOut = Liquid::In;
};

#endif // LIQUID_P_H
