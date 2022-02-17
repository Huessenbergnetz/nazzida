/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "dailyliquids_p.h"

DailyLiquids::DailyLiquids()
    : d(new DailyLiquidsData)
{

}

DailyLiquids::DailyLiquids(int id, int personId, QDate day, int input, int output)
    : d(new DailyLiquidsData(id, personId, day, input, output))
{

}

DailyLiquids::DailyLiquids(const DailyLiquids &other) = default;

DailyLiquids::DailyLiquids(DailyLiquids &&other) noexcept = default;

DailyLiquids& DailyLiquids::operator=(const DailyLiquids &other) = default;

DailyLiquids& DailyLiquids::operator=(DailyLiquids &&other) noexcept = default;

DailyLiquids::~DailyLiquids()
{

}

void DailyLiquids::swap(DailyLiquids &other) noexcept
{
    std::swap(d, other.d);
}

int DailyLiquids::id() const
{
    return d->id;
}

int DailyLiquids::personId() const
{
    return d->personId;
}

QDate DailyLiquids::day() const
{
    return d->day;
}

int DailyLiquids::input() const
{
    return d->input;
}

int DailyLiquids::output() const
{
    return d->output;
}

int DailyLiquids::difference() const
{
    return d->input - d->output;
}

#include "moc_dailyliquids.cpp"
