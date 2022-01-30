/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef LIQUID_H
#define LIQUID_H

#include <QObject>
#include <QDateTime>
#include <QSharedDataPointer>

class LiquidData;

class Liquid
{
    Q_GADGET
    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(int personId READ personId CONSTANT)
    Q_PROPERTY(QDateTime moment READ moment CONSTANT)
    Q_PROPERTY(Liquid::InOrOut inOrOut READ inOrOut CONSTANT)
    Q_PROPERTY(int amount READ amount CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString note READ note CONSTANT)
public:
    enum InOrOut {
        Out = 0,
        In = 1,
        InIV = 2
    };
    Q_ENUM(InOrOut)

    Liquid();
    Liquid(int id, int personId, const QDateTime &moment, Liquid::InOrOut inOrOut, int amount, const QString &name, const QString &note);
    Liquid(const Liquid &other);
    Liquid(Liquid &&other) noexcept;
    Liquid& operator=(const Liquid &other);
    Liquid& operator=(Liquid &&other) noexcept;
    ~Liquid();

    void swap(Liquid &other) noexcept;

    int id() const;

    int personId() const;

    QDateTime moment() const;

    InOrOut inOrOut() const;

    int amount() const;

    QString name() const;

    QString note() const;

private:
    friend class LiquidListModel;
    QSharedDataPointer<LiquidData> d;
};

Q_DECLARE_METATYPE(Liquid)
Q_DECLARE_TYPEINFO(Liquid, Q_MOVABLE_TYPE);

#endif // LIQUID_H
