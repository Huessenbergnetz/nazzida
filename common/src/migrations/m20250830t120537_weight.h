/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef M20250830T120537_WEIGHT_H
#define M20250830T120537_WEIGHT_H

#include <Firfuorida/Migration>

class M20250830T120537_Weight final : public Firfuorida::Migration
{
    Q_OBJECT
    Q_DISABLE_COPY(M20250830T120537_Weight)
public:
    explicit M20250830T120537_Weight(Firfuorida::Migrator *parent);
    ~M20250830T120537_Weight() override = default;

    void up() final;
    void down() final;
};

#endif // M20250830T120537_WEIGHT_H

