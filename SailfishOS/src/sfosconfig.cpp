/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "sfosconfig.h"
#include <QSettings>

SfosConfig::SfosConfig(QObject *parent) : Configuration(parent)
{
    m_config = new QSettings(this);
}

SfosConfig::~SfosConfig()
{

}

QString SfosConfig::language() const
{
    return m_config->value(QStringLiteral("display/language"), QString()).toString();
}

void SfosConfig::setLanguage(const QString &lang)
{
    const QString currentLanguage = language();
    if (currentLanguage != lang) {
        m_config->setValue(QStringLiteral("display/language"), lang);
        emit languageChanged(language());
    }
}

#include "moc_sfosconfig.cpp"
