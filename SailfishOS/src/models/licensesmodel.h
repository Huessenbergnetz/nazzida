/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef LICENSESMODEL_H
#define LICENSESMODEL_H

#include <QObject>
#include <hbnsclicensemodel.h>

class LicensesModel : public Hbnsc::LicenseModel
{
    Q_OBJECT
    Q_DISABLE_COPY(LicensesModel)
public:
    explicit LicensesModel(QObject *parent = nullptr);
    ~LicensesModel() override;
};

#endif // LICENSESMODEL_H
