/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "licensesmodel.h"

LicensesModel::LicensesModel(QObject *parent)
    : Hbnsc::LicenseModel(parent)
{

}

LicensesModel::~LicensesModel()
{

}

#include "moc_licensesmodel.cpp"
