/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "sfosconfig.h"
#include <QSettings>
#include <QStandardPaths>
#include <QCoreApplication>

SfosConfig::SfosConfig(QObject *parent) : Configuration(parent)
{
    m_config = new QSettings(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + QCoreApplication::organizationName() + QLatin1Char('/') + QCoreApplication::applicationName() + QLatin1Char('/') + QCoreApplication::applicationName() + QStringLiteral(".conf"), QSettings::NativeFormat, this);
    qDebug("Reading settings from %s", qUtf8Printable(m_config->fileName()));
    if (m_config->status() != QSettings::NoError) {
        qWarning("Failed to read setting from %s", qUtf8Printable(m_config->fileName()));
    }
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
        qDebug("Changing display/language from \"%s\" to \"%s\"", qUtf8Printable(currentLanguage), qUtf8Printable(lang));
        m_config->setValue(QStringLiteral("display/language"), lang);
        emit languageChanged(language());
    }
}

#include "moc_sfosconfig.cpp"
