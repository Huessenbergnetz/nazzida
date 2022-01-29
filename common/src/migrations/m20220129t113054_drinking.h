/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef M20220129T113054_DRINKING_H
#define M20220129T113054_DRINKING_H

#include <Firfuorida/migration.h>

class M20220129T113054_Drinking : public Firfuorida::Migration
{
    Q_OBJECT
    Q_DISABLE_COPY(M20220129T113054_Drinking)
public:
    explicit M20220129T113054_Drinking(Firfuorida::Migrator *parent);
    ~M20220129T113054_Drinking() override;

    void up() override;
    void down() override;
};

#endif // M20220129T113054_DRINKING_H

