/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef DAILYLIQUIDS_P_H
#define DAILYLIQUIDS_P_H

#include "dailyliquids.h"
#include <QSharedData>

class DailyLiquidsData : public QSharedData
{
public:
    DailyLiquidsData() : QSharedData() {}

    DailyLiquidsData(int _id, int _personId, QDate _day, int _input, int _output) :
        QSharedData(),
        day(_day),
        id(_id),
        personId(_personId),
        input(_input),
        output(_output)
    {}

    ~DailyLiquidsData() {}

    QDate day;
    int id = 0;
    int personId = 0;
    int input = 0;
    int output = 0;
};

#endif // DAILYLIQUIDS_P_H
