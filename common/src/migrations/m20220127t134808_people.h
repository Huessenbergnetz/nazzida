/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef M20220127T134808_PEOPLE_H
#define M20220127T134808_PEOPLE_H

#include <Firfuorida/migration.h>

class M20220127T134808_People : public Firfuorida::Migration
{
    Q_OBJECT
    Q_DISABLE_COPY(M20220127T134808_People)
public:
    explicit M20220127T134808_People(Firfuorida::Migrator *parent);
    ~M20220127T134808_People() override;

    void up() override;
    void down() override;
};

#endif // M20220127T134808_PEOPLE_H

