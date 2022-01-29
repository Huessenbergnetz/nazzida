/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "m20220129t113054_drinking.h"

M20220129T113054_Drinking::M20220129T113054_Drinking(Firfuorida::Migrator *parent) :
    Firfuorida::Migration(parent)
{

}

M20220129T113054_Drinking::~M20220129T113054_Drinking()
{

}

void M20220129T113054_Drinking::up()
{
    auto t = create(QStringLiteral("drinking"));
    t->increments();
    t->integer(QStringLiteral("person_id"))->unSigned();
    t->dateTime(QStringLiteral("moment"))->defaultValue(QStringLiteral("CURRENT_TIMESTAMP"));
    t->tinyInteger(QStringLiteral("in_or_out"))->defaultValue(1);
    t->integer(QStringLiteral("amount"))->unSigned();
    t->varChar(QStringLiteral("name"));
    t->text(QStringLiteral("note"))->nullable();

    t->foreignKey(QStringLiteral("person_id"), QStringLiteral("people"), QStringLiteral("id"), QStringLiteral("drinking_person_id"))->onUpdate(QStringLiteral("CASCADE"))->onDelete(QStringLiteral("CASCADE"));
}

void M20220129T113054_Drinking::down()
{
    drop(QStringLiteral("drinking"));
}

#include "moc_m20220129t113054_drinking.cpp"

