/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "liquid_p.h"

Liquid::Liquid()
    : d(new LiquidData)
{

}

Liquid::Liquid(int id, int personId, const QDateTime &moment, Liquid::InOrOut inOrOut, int amount, const QString &name, const QString &note)
    : d(new LiquidData(id, personId, moment, inOrOut, amount, name, note))
{

}

Liquid::Liquid(const Liquid &other) = default;

Liquid::Liquid(Liquid &&other) noexcept = default;

Liquid& Liquid::operator=(const Liquid &other) = default;

Liquid& Liquid::operator=(Liquid &&other) noexcept = default;

Liquid::~Liquid()
{

}

void Liquid::swap(Liquid &other) noexcept
{
    std::swap(d, other.d);
}

int Liquid::id() const
{
    return d->id;
}

int Liquid::personId() const
{
    return d->personId;
}

QDateTime Liquid::moment() const
{
    return d->moment;
}

Liquid::InOrOut Liquid::inOrOut() const
{
    return d->inOrOut;
}

int Liquid::amount() const
{
    return d->amount;
}

QString Liquid::name() const
{
    return d->name;
}

QString Liquid::note() const
{
    return d->note;
}

#include "moc_liquid.cpp"
