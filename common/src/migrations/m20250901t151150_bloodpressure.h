/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef M20250901T151150_BLOODPRESSURE_H
#define M20250901T151150_BLOODPRESSURE_H

#include <Firfuorida/Migration>

class M20250901T151150_BloodPressure final : public Firfuorida::Migration
{
    Q_OBJECT
    Q_DISABLE_COPY(M20250901T151150_BloodPressure)
public:
    explicit M20250901T151150_BloodPressure(Firfuorida::Migrator *parent);
    ~M20250901T151150_BloodPressure() override = default;

    void up() final;
    void down() final;
};

#endif // M20250901T151150_BLOODPRESSURE_H

