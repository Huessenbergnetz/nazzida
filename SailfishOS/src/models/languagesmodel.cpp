/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "languagesmodel.h"

LanguagesModel::LanguagesModel(QObject *parent) :
    Hbnsc::LanguageModel({QStringLiteral("en_GB"), QStringLiteral("en_US"), QStringLiteral("de"), QStringLiteral("sv")}, parent)
{

}

LanguagesModel::~LanguagesModel()
{

}

#include "moc_languagesmodel.cpp"
