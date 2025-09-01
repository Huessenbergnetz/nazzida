/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "m20250901t134046_sysinfo.h"

M20250901T134046_SysInfo::M20250901T134046_SysInfo(Firfuorida::Migrator *parent)
    : Firfuorida::Migration{parent}
{

}

void M20250901T134046_SysInfo::up()
{
    auto t = create(QStringLiteral("sysinfo"));
    t->text(QStringLiteral("name"))->unique();
    t->text(QStringLiteral("value"));
}

void M20250901T134046_SysInfo::down()
{
    drop(QStringLiteral("sysinfo"));
}

#include "moc_m20250901t134046_sysinfo.cpp"

