/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef SFOSCONFIG_H
#define SFOSCONFIG_H

#include "configuration.h"

class QSettings;

class SfosConfig : public Configuration
{
    Q_OBJECT
    Q_DISABLE_COPY(SfosConfig)
public:
    explicit SfosConfig(QObject *parent = nullptr);
    ~SfosConfig() override;

    QString language() const override;
    void setLanguage(const QString &lang) override;

private:
    QSettings *m_config;
};

#endif // SFOSCONFIG_H
