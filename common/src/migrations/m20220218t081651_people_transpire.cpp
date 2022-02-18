/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "m20220218t081651_people_transpire.h"

M20220218T081651_People_transpire::M20220218T081651_People_transpire(Firfuorida::Migrator *parent) :
    Firfuorida::Migration(parent)
{

}

M20220218T081651_People_transpire::~M20220218T081651_People_transpire()
{

}

void M20220218T081651_People_transpire::up()
{
    auto t = table(QStringLiteral("people"));
    t->integer(QStringLiteral("transpire"))->defaultValue(500);
}

void M20220218T081651_People_transpire::down()
{
    auto t = table(QStringLiteral("people"));
    t->dropColumn(QStringLiteral("transpire"));
}

#include "moc_m20220218t081651_people_transpire.cpp"

