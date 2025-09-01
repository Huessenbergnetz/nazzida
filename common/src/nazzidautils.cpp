/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "nazzidautils.h"

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
}

int NazzidaUtils::ageAtMoment(Person *person, const QDateTime &moment) const
{
    const qint64 days = person->birthday().daysTo(moment.date());
    return std::abs(std::floor(days/365.25));
}

#include "moc_nazzidautils.cpp"
