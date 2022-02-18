/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef M20220218T081651_PEOPLE_TRANSPIRE_H
#define M20220218T081651_PEOPLE_TRANSPIRE_H

#include <Firfuorida/migration.h>

class M20220218T081651_People_transpire : public Firfuorida::Migration
{
    Q_OBJECT
    Q_DISABLE_COPY(M20220218T081651_People_transpire)
public:
    explicit M20220218T081651_People_transpire(Firfuorida::Migrator *parent);
    ~M20220218T081651_People_transpire() override;

    void up() override;
    void down() override;
};

#endif // M20220218T081651_PEOPLE_TRANSPIRE_H

