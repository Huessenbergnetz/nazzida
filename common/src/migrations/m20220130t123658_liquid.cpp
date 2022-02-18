/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "m20220130t123658_liquid.h"

M20220130T123658_Liquid::M20220130T123658_Liquid(Firfuorida::Migrator *parent) :
    Firfuorida::Migration(parent)
{

}

M20220130T123658_Liquid::~M20220130T123658_Liquid()
{

}

void M20220130T123658_Liquid::up()
{
    auto t = create(QStringLiteral("liquid"));
    t->increments();
    t->integer(QStringLiteral("person_id"))->unSigned();
    t->dateTime(QStringLiteral("moment"))->defaultValue(QStringLiteral("CURRENT_TIMESTAMP"));
    t->tinyInteger(QStringLiteral("in_or_out"))->defaultValue(1);
    t->integer(QStringLiteral("amount"))->unSigned();
    t->varChar(QStringLiteral("name"));
    t->text(QStringLiteral("note"))->nullable();

    t->foreignKey(QStringLiteral("person_id"), QStringLiteral("people"), QStringLiteral("id"), QStringLiteral("drinking_person_id"))->onUpdate(QStringLiteral("CASCADE"))->onDelete(QStringLiteral("CASCADE"));
}

void M20220130T123658_Liquid::down()
{
    drop(QStringLiteral("liquid"));
}

#include "moc_m20220130t123658_liquid.cpp"

