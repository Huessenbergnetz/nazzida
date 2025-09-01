/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "m20250901t151150_bloodpressure.h"

M20250901T151150_BloodPressure::M20250901T151150_BloodPressure(Firfuorida::Migrator *parent)
    : Firfuorida::Migration{parent}
{

}

void M20250901T151150_BloodPressure::up()
{
    auto t = create(QStringLiteral("bloodpressure"));
    t->increments();
    t->integer(QStringLiteral("person_id"))->unSigned();
    t->dateTime(QStringLiteral("moment"))->defaultValue(QStringLiteral("CURRENT_TIMESTAMP"));
    t->integer(QStringLiteral("systolic"));
    t->integer(QStringLiteral("diastolic"));
    t->integer(QStringLiteral("pulse"));
    t->text(QStringLiteral("note"))->nullable();

    t->foreignKey(QStringLiteral("person_id"), QStringLiteral("people"), QStringLiteral("id"), QStringLiteral("bloodpressure_person_id"))->onUpdate(QStringLiteral("CASCADE"))->onDelete(QStringLiteral("CASCADE"));

}

void M20250901T151150_BloodPressure::down()
{
    drop(QStringLiteral("bloodpressure"));
}

#include "moc_m20250901t151150_bloodpressure.cpp"

