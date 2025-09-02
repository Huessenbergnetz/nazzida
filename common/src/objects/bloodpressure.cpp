/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "bloodpressure_p.h"
#include "../nazzidautils.h"

BloodPressure::BloodPressure()
    : d(new BloodPressureData)
{
}

BloodPressure::BloodPressure(int id, int personId, const QDateTime &moment, int systolic, int diastolic, int pulse, const QString &note)
    : d(new BloodPressureData(id, personId, moment, systolic, diastolic, pulse, note))
{
}

BloodPressure::BloodPressure(const BloodPressure &other) = default;

BloodPressure::BloodPressure(BloodPressure &&other) noexcept = default;

BloodPressure& BloodPressure::operator=(const BloodPressure &other) = default;

BloodPressure& BloodPressure::operator=(BloodPressure &&other) noexcept = default;

BloodPressure::~BloodPressure() = default;

void BloodPressure::swap(BloodPressure &other) noexcept
{
    std::swap(d, other.d);
}

int BloodPressure::id() const
{
    return d->id;
}

int BloodPressure::personId() const
{
    return d->personId;
}

QDateTime BloodPressure::moment() const
{
    return d->moment;
}

int BloodPressure::systolic() const
{
    return d->systolic;
}

int BloodPressure::diastolic() const
{
    return d->diastolic;
}

int BloodPressure::pulse() const
{
    return d->pulse;
}

QString BloodPressure::note() const
{
    return d->note;
}

QString BloodPressure::relDate() const
{
    return NazzidaUtils::relDate(d->moment.date());
}

#include "moc_bloodpressure.cpp"
