/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "nazzidautils.h"
#include "configuration.h"

#include <QLocale>

#include <cmath>

NazzidaUtils::NazzidaUtils(QObject *parent) : QObject(parent)
{

}

NazzidaUtils::~NazzidaUtils() = default;

double NazzidaUtils::calcBmi(Person *person, int weight) const
{
    const double sizeInM    = static_cast<double>(person->size()) / 100.0;
    const double size2      = sizeInM * sizeInM;
    const double weightInKg = static_cast<double>(weight) / 1'000.0;
    return weightInKg / size2;
}

NazzidaUtils::BmiGeneral NazzidaUtils::bmiGeneral(Person *person, double bmi) const
{
    Q_UNUSED(person)
    if (bmi < 18.50) {
        return Underweight;
    } else if (bmi >= 18.50 && bmi < 25.00) {
        return Normalweight;
    } else if (bmi >= 25.00 && bmi < 30.00) {
        return Overweight;
    } else {
        return Obese;
    }
}

QString NazzidaUtils::bmiGeneralString(Person *person, double bmi) const
{
    const BmiGeneral classification = bmiGeneral(person, bmi);
    switch (classification) {
    case Underweight:
        //: BMI general classification
        //% "Underweight"
        return qtTrId("naz-bmi-general-underweight");
    case Normalweight:
        //: BMI general classification
        //% "Normal weight"
        return qtTrId("naz-bmi-general-normal");
    case Overweight:
        //: BMI general classification
        //% "Overweight"
        return qtTrId("naz-bmi-general-overweight");
    case Obese:
        //: BMI general classification
        //% "Obese"
        return qtTrId("naz-bmi-general-obese");
    }

    return QString();
}

NazzidaUtils::BmiDetailed NazzidaUtils::bmiDetailed(Person *person, double bmi) const
{
    Q_UNUSED(person);
    if (bmi < 16.00) {
        return SevereThinness;
    } else if (bmi >= 16.00 && bmi < 17.00) {
        return ModerateThinness;
    } else if (bmi >= 17.00 && bmi < 18.50) {
        return MildThinness;
    } else if (bmi >= 18.50 && bmi < 25.00) {
        return Normal;
    } else if (bmi >= 25.00 && bmi < 30.00) {
        return PreObese;
    } else if (bmi >= 30.00 && bmi < 35.00) {
        return ObeseClass1;
    } else if (bmi >= 35.00 && bmi < 40.00) {
        return ObeseClass2;
    } else {
        return ObeseClass3;
    }
}

QString NazzidaUtils::bmiDetailedString(Person *person, double bmi) const
{
    const BmiDetailed classification = bmiDetailed(person, bmi);
    switch(classification) {
    case SevereThinness:
        //: BMI detailed classification
        //% "Severe thinness"
        return qtTrId("naz-bmi-detailed-severe-thinness");
    case ModerateThinness:
        //: BMI detailed classification
        //% "Moderate thinness"
        return qtTrId("naz-bmi-detailed-moderate-thinness");
    case MildThinness:
        //: BMI detailed classification
        //% "Mild thinness"
        return qtTrId("naz-bmi-detailed-mild-thinness");
    case Normal:
        //: BMI detailed classification
        //% "Normal weight"
        return qtTrId("naz-bmi-detailed-normal");
    case PreObese:
        //: BMI detailed classification
        //% "Pre-obese"
        return qtTrId("naz-bmi-detailed-pre-obese");
    case ObeseClass1:
        //: BMI detailed classification
        //% "Obese class I"
        return qtTrId("naz-bmi-detailed-obese-1");
    case ObeseClass2:
        //: BMI detailed classification
        //% "Obese class II"
        return qtTrId("naz-bmi-detailed-obese-2");
    case ObeseClass3:
        //: BMI detailed classification
        //% "Obese class III"
        return qtTrId("naz-bmi-detailed-obese-3");
    }

    return QString();
}

int NazzidaUtils::ageAtMoment(Person *person, const QDateTime &moment) const
{
    const qint64 days = person->birthday().daysTo(moment.date());
    return std::abs(std::floor(days/365.25));
}

QString NazzidaUtils::relDate(QDate date)
{
    const auto today    = QDate::currentDate();
    const qint64 daysTo = date.daysTo(today);

    if (daysTo == 0) {
        //: relative date
        //% "Today"
        return qtTrId("naz-reldate-today");
    } else if (daysTo == 1) {
        //: relative date
        //% "Yesterday"
        return qtTrId("naz-reldate-yesterday");
    } else if (daysTo > 1 && daysTo < 7) {
        QLocale l;
        return l.dayName(date.dayOfWeek());
    } else {
        QLocale l;
        return l.toString(date, QLocale::ShortFormat);
    }
}

QString NazzidaUtils::bpClassString(Person *person, int bpClass, int sys, int dia) const
{
    const auto _bpClass = static_cast<Configuration::BpClass>(bpClass);

    switch(_bpClass) {
    case Configuration::AccAhh:
        return bpAccAhhString(person, sys, dia);
    case Configuration::Esc:
        return bpEscString(person, sys, dia);
    case Configuration::EshIsh:
        return bpEshIshString(person, sys, dia);
    }

    return QString();
}

QString NazzidaUtils::bpAccAhhString(Person *person, int sys, int dia) const
{
    Q_UNUSED(person)

    if (sys < 120 && dia < 80) {
        //: normal blood pressure according to ACC/AHH
        //% "Normal"
        return qtTrId("naz-bp-accahh-normal");
    } else if ((sys >= 120 && sys < 130) && (dia < 80)) {
        //: elevated blood pressure according to ACC/AHH
        //% "Elevated"
        return qtTrId("naz-bp-accahh-elevated");
    } else if ((sys >= 130 && sys < 135) || (dia >= 80 && dia < 85)) {
        //: hypertension stage 1 according to ACC/AHH
        //% "Hypertension, stage 1"
        return qtTrId("naz-bp-accahh-hypertension-1");
    } else if (sys >= 130 || dia >= 85) {
        //: hypertension stage 2 according to ACC/AHH
        //% "Hypertension, stage 2"
        return qtTrId("naz-bp-accahh-hypertension-2");
    }

    return QString();
}

QString NazzidaUtils::bpEscString(Person *person, int sys, int dia) const
{
    Q_UNUSED(person)

    if (sys < 120 && dia < 70) {
        //: non-elevated blood pressure according to ESC
        //% "Non-elevated"
        return qtTrId("naz-bp-esc-non-elevated");
    } else if ((sys >= 120 && sys < 135) && (dia >= 70 && dia < 85)) {
        //: elevated blood pressure according to ESC
        //% "Elevated"
        return qtTrId("naz-bp-esc-elevated");
    } else if (sys >= 135 || dia >= 85) {
        //: hypertension blood pressure according to ESC
        //% "Hypertension"
        return qtTrId("naz-bp-esc-hypertension");
    }

    return QString();
}

QString NazzidaUtils::bpEshIshString(Person *person, int sys, int dia) const
{
    Q_UNUSED(person)

    if (sys < 120 && dia < 80) {
        //: optimal blood pressure according to ESH/ISH
        //% "Optimal"
        return qtTrId("baz-bp-eshish-optimal");
    } else if ((sys >= 120 && sys < 130) || (dia >= 80 && dia < 85)) {
        //: normal blood pressure according to ESH/ISH
        //% "Normal"
        return qtTrId("baz-bp-eshish-normal");
    } else if ((sys >= 130 && sys < 140) || (dia >= 85 && dia < 90)) {
        //: high normal blood pressure according to ESH/ISH
        //% "High normal"
        return qtTrId("baz-bp-eshish-high-normal");
    } else if ((sys >= 140 && sys < 160) || (dia >= 90 && dia < 100)) {
        //: hypertension grade 1 blood pressure accordinhg to ESH/ISH
        //% "Hypertension, grade 1"
        return qtTrId("baz-bp-eshish-hypertension-1");
    } else if ((sys >= 160 && sys < 180) || (dia >= 100 && dia < 110)) {
        //: hypertension grade 2 blood pressure accordinhg to ESH/ISH
        //% "Hypertension, grade 2"
        return qtTrId("baz-bp-eshish-hypertension-2");
    } else if (sys >= 180 || dia >= 110) {
        //: hypertension grade 3 blood pressure accordinhg to ESH/ISH
        //% "Hypertension, grade 3"
        return qtTrId("baz-bp-eshish-hypertension-3");
    }

    return QString();
}

#include "moc_nazzidautils.cpp"
