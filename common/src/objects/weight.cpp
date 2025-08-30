/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "weight_p.h"

Weight::Weight()
    : d(new WeightData)
{
}

Weight::Weight(int id, int personId, const QDateTime &moment, int weight, const QString &note)
    : d(new WeightData(id, personId, moment, weight, note))
{
}

Weight::Weight(const Weight &other) = default;

Weight::Weight(Weight &&other) noexcept = default;

Weight& Weight::operator=(const Weight &other) = default;

Weight& Weight::operator=(Weight &&other) noexcept = default;

Weight::~Weight()
{
}

void Weight::swap(Weight &other) noexcept
{
    std::swap(d, other.d);
}

int Weight::id() const
{
    return d->id;
}

int Weight::personId() const
{
    return d->personId;
}

QDateTime Weight::moment() const
{
    return d->moment;
}

int Weight::weight() const
{
    return d->weight;
}

QString Weight::note() const
{
    return d->note;
}

#include "moc_weight.cpp"
