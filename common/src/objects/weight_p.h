/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef WEIGHT_P_H
#define WEIGHT_P_H

#include "weight.h"
#include <QSharedData>

class WeightData : public QSharedData
{
public:
    WeightData() : QSharedData() {}

    WeightData(int _id, int _personId, const QDateTime &_moment, int _weight, QString _note) :
        QSharedData(),
        note{_note},
        moment{_moment},
        id{_id},
        personId{_personId},
        weight{_weight}
    {}

    ~WeightData() {}

    QString note;
    QDateTime moment;
    int id{0};
    int personId{0};
    int weight{0};
};

#endif // WEIGHT_P_H
