/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef BLOODPRESSURE_P_H
#define BLOODPRESSURE_P_H

#include "bloodpressure.h"
#include <QSharedData>

class BloodPressureData : public QSharedData
{
public:
    BloodPressureData() : QSharedData()  {}

    BloodPressureData(int _id, int _personId, const QDateTime &_moment, int _systolic, int _diastolic, int _pulse, const QString &_note)
        : QSharedData()
        , note{_note}
        , moment{_moment}
        , id{_id}
        , personId{_personId}
        , systolic{_systolic}
        , diastolic{_diastolic}
        , pulse{_pulse}
    {}

    ~BloodPressureData() = default;

    QString note;
    QDateTime moment;
    int id{0};
    int personId{0};
    int systolic{0};
    int diastolic{0};
    int pulse{0};
};

#endif // BLOODPRESSURE_P_H
