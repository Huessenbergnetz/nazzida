/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "sfosconfig.h"
#include <QSettings>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QDebug>

SfosConfig::SfosConfig(QObject *parent) : Configuration(parent)
{
    m_config = new QSettings(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + QCoreApplication::organizationName() + QLatin1Char('/') + QCoreApplication::applicationName() + QLatin1Char('/') + QCoreApplication::applicationName() + QStringLiteral(".conf"), QSettings::NativeFormat, this);
    qDebug() << "Reading settings from" << m_config->fileName();
    if (m_config->status() != QSettings::NoError) {
        qWarning() << "Failed to read settings from" << m_config->fileName();
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
        qDebug() << "Changing display/language from" << currentLanguage << "to" << lang;
        m_config->setValue(QStringLiteral("display/language"), lang);
        emit languageChanged(language());
    }
}

Configuration::BpClass SfosConfig::bpClass() const
{
    return static_cast<Configuration::BpClass>(m_config->value(QStringLiteral("classifications/bloodpressure"), static_cast<int>(Configuration::EshIsh)).toInt());
}

void SfosConfig::setBpClass(Configuration::BpClass bpClass)
{
    const auto currentBpClass = this->bpClass();
    if (currentBpClass != bpClass) {
        qDebug() << "Changing classification/bloodpressure from" << currentBpClass << "to" << bpClass;
        m_config->setValue(QStringLiteral("classifications/bloodpressure"), static_cast<int>(bpClass));
        emit bpClassChanged(this->bpClass());
    }
}

#include "moc_sfosconfig.cpp"
