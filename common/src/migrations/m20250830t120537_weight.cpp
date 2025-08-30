/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "m20250830t120537_weight.h"

M20250830T120537_Weight::M20250830T120537_Weight(Firfuorida::Migrator *parent)
    : Firfuorida::Migration{parent}
{

}

void M20250830T120537_Weight::up()
{
    auto t = create(QStringLiteral("weight"));
    t->increments();
    t->integer(QStringLiteral("person_id"))->unSigned();
    t->dateTime(QStringLiteral("moment"))->defaultValue(QStringLiteral("CURRENT_TIMESTAMP"));
    t->integer(QStringLiteral("weight"))->unSigned();
    t->text(QStringLiteral("note"))->nullable();

    t->foreignKey(QStringLiteral("person_id"), QStringLiteral("people"), QStringLiteral("id"), QStringLiteral("weight_person_id"))->onUpdate(QStringLiteral("CASCADE"))->onDelete(QStringLiteral("CASCADE"));
}

void M20250830T120537_Weight::down()
{
    drop(QStringLiteral("weight"));
}

#include "moc_m20250830t120537_weight.cpp"

