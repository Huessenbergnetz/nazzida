/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "m20220127t134808_people.h"

M20220127T134808_People::M20220127T134808_People(Firfuorida::Migrator *parent) :
    Firfuorida::Migration(parent)
{

}

M20220127T134808_People::~M20220127T134808_People()
{

}

void M20220127T134808_People::up()
{
    auto t = create(QStringLiteral("people"));
    t->increments();
    t->varChar(QStringLiteral("first_name"));
    t->varChar(QStringLiteral("last_name"));
    t->tinyInteger(QStringLiteral("size"));
    t->mediumInteger(QStringLiteral("weight"));
    t->date(QStringLiteral("birthday"));
    t->time(QStringLiteral("day_starts"));
    t->charCol(QStringLiteral("sex"), 1);
}

void M20220127T134808_People::down()
{
    drop(QStringLiteral("people"));
}

#include "moc_m20220127t134808_people.cpp"

