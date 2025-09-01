/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef BLOODPRESSURE_H
#define BLOODPRESSURE_H

#include <QObject>
#include <QDateTime>
#include <QSharedDataPointer>

class BloodPressureData;

class BloodPressure
{
    Q_GADGET
    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(int personId READ personId CONSTANT)
    Q_PROPERTY(QDateTime moment READ moment CONSTANT)
    Q_PROPERTY(int systolic READ systolic CONSTANT)
    Q_PROPERTY(int diastolic READ diastolic CONSTANT)
    Q_PROPERTY(int pulse READ pulse CONSTANT)
    Q_PROPERTY(QString note READ note CONSTANT)
public:
    BloodPressure();
    BloodPressure(int id, int personId, const QDateTime &moment, int systolic, int diastolic, int pulse, const QString &note);
    BloodPressure(const BloodPressure &other);
    BloodPressure(BloodPressure &&ohter) noexcept;
    BloodPressure& operator=(const BloodPressure &other);
    BloodPressure& operator=(BloodPressure &&other) noexcept;
    ~BloodPressure();

    void swap(BloodPressure &other) noexcept;

    int id() const;

    int personId() const;

    QDateTime moment() const;

    int systolic() const;

    int diastolic() const;

    int pulse() const;

    QString note() const;

private:
    friend class BloodPressureListModel;
    QSharedDataPointer<BloodPressureData> d;
};

Q_DECLARE_METATYPE(BloodPressure)
Q_DECLARE_TYPEINFO(BloodPressure, Q_MOVABLE_TYPE);

#endif // BLOODPRESSURE_H
