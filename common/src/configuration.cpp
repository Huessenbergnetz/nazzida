/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "configuration.h"

Configuration::Configuration(QObject *parent) : QObject(parent)
{

}

Configuration::~Configuration()
{

}

QString Configuration::language() const
{
    return QStringLiteral("C");
}

void Configuration::setLanguage(const QString &language)
{
    Q_UNUSED(language)
}

Configuration::BpClass Configuration::bpClass() const
{
    return EshIsh;
}

void Configuration::setBpClass(BpClass bpClass)
{
    Q_UNUSED(bpClass);
}

#include "moc_configuration.cpp"
