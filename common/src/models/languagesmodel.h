/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef LANGUAGESMODEL_H
#define LANGUAGESMODEL_H

#include <QObject>
#include <hbnsclanguagemodel.h>

class LanguagesModel : public Hbnsc::LanguageModel
{
    Q_OBJECT
    Q_DISABLE_COPY(LanguagesModel)
public:
    explicit LanguagesModel(QObject *parent = nullptr);
    ~LanguagesModel() override;
};

#endif // LANGUAGESMODEL_H
