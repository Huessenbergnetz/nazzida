/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef WEIGHT_H
#define WEIGHT_H

#include <QObject>
#include <QDateTime>
#include <QSharedDataPointer>

class WeightData;

class Weight
{
    Q_GADGET
    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(int personId READ personId CONSTANT)
    Q_PROPERTY(QDateTime moment READ moment CONSTANT)
    Q_PROPERTY(int weight READ weight CONSTANT)
    Q_PROPERTY(QString note READ note CONSTANT)
public:
    Weight();
    Weight(int id, int personId, const QDateTime &moment, int weight, const QString &note);
    Weight(const Weight &other);
    Weight(Weight &&other) noexcept;
    Weight& operator=(const Weight &other);
    Weight& operator=(Weight &&other) noexcept;
    ~Weight();

    void swap(Weight &other) noexcept;

    int id() const;

    int personId() const;

    QDateTime moment() const;

    int weight() const;

    QString note() const;

private:
    friend class WeightListModel;
    QSharedDataPointer<WeightData> d;
};

Q_DECLARE_METATYPE(Weight)
Q_DECLARE_TYPEINFO(Weight, Q_MOVABLE_TYPE);

#endif // WEIGHT_H
