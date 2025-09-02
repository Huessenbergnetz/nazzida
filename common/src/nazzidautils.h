/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef NAZZIDAUTILS_H
#define NAZZIDAUTILS_H

#include "objects/person.h"

#include <QDateTime>
#include <QObject>

class QQmlEngine;
class QJSEngine;

class NazzidaUtils : public QObject
{
    Q_OBJECT
public:
    explicit NazzidaUtils(QObject *parent = nullptr);
    ~NazzidaUtils() override;

    enum BmiGeneral : int {
        Underweight = -1,
        Normalweight = 0,
        Overweight = 1,
        Obese = 2
    };
    Q_ENUM(BmiGeneral)

    enum BmiDetailed : int {
        SevereThinness = -3,
        ModerateThinness = -2,
        MildThinness = -1,
        Normal = 0,
        PreObese = 1,
        ObeseClass1 = 2,
        ObeseClass2 = 3,
        ObeseClass3 = 4
    };
    Q_ENUM(BmiDetailed)

    enum BpAccAhh : int {
        BpAccAhhNormal = 0,
        BpAccAhhElevated = 1,
        BpAccAhhHypertension1 = 2,
        BpAccAhhHypertension2 = 3
    };
    Q_ENUM(BpAccAhh);

    enum BpEsc : int {
        EscNonElevated  = 0,
        EscElevated     = 1,
        EscHypertension = 2
    };
    Q_ENUM(BpEsc);

    enum EshIsh : int {
        EshIshOptimal = 0,
        EshIshNormal = 1,
        EshIshHighNormal = 2,
        EshIshHypertension1 = 3,
        EshIshHypertension2 = 4,
        EshIshHypertension3 = 5
    };
    Q_ENUM(EshIsh);

    Q_INVOKABLE double calcBmi(Person *person, int weight) const;

    Q_INVOKABLE BmiGeneral bmiGeneral(Person *person, double bmi) const;
    Q_INVOKABLE QString bmiGeneralString(Person *person, double bmi) const;

    Q_INVOKABLE BmiDetailed bmiDetailed(Person *person, double bmi) const;
    Q_INVOKABLE QString bmiDetailedString(Person *person, double bmi) const;

    Q_INVOKABLE int ageAtMoment(Person *person, const QDateTime &moment) const;

    Q_INVOKABLE QString bpClassString(Person *person, int bpClass, int sys, int dia) const;
    Q_INVOKABLE QString bpAccAhhString(Person *person, int sys, int dia) const;
    Q_INVOKABLE QString bpEscString(Person *person, int sys, int dia) const;
    Q_INVOKABLE QString bpEshIshString(Person *person, int sys, int dia) const;

    static QObject *provider(QQmlEngine *engine, QJSEngine *scriptEngine)
    {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return new NazzidaUtils();
    }

    static QString relDate(QDate date);

private:
    Q_DISABLE_COPY(NazzidaUtils)
};

#endif // NAZZIDAUTILS_H
