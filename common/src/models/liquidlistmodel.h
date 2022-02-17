/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef LIQUIDLISTMODEL_H
#define LIQUIDLISTMODEL_H

#include "baselistmodel.h"
#include "../objects/liquid.h"
#include <QObject>
#include <QTime>
#include <QDate>
#include <vector>

class LiquidListModel : public BaseListModel
{
    Q_OBJECT
    Q_PROPERTY(int personId READ personId WRITE setPersonId NOTIFY personIdChanged)
    Q_PROPERTY(QTime dayStarts READ dayStarts WRITE setDayStarts NOTIFY dayStartsChanged)
    Q_PROPERTY(QDate day READ day WRITE setDay NOTIFY dayChanged)
    Q_PROPERTY(int difference READ difference NOTIFY differenceChanged)
public:
    explicit LiquidListModel(QObject *parent = nullptr);
    ~LiquidListModel() override;

    enum Roles {
        Id = Qt::UserRole + 1,
        PersonId,
        Moment,
        InOrOut,
        Amount,
        Name,
        Note
    };
    Q_ENUM(Roles)

    Q_INVOKABLE bool load() override;
    Q_INVOKABLE bool loadForPerson(int personId, QTime dayStarts, QDate day);
    Q_INVOKABLE void clear() override;

    Q_INVOKABLE int add(int id, const QDateTime &moment, int inOrOut, int amount, const QString &name, const QString &note);
    Q_INVOKABLE bool remove(QModelIndex index);
    Q_INVOKABLE bool edit(QModelIndex index, const QDateTime &moment, int inOrOut, int amount, const QString &name, const QString &note);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    int personId() const;
    void setPersonId(int id);

    QTime dayStarts() const;
    void setDayStarts(QTime start);

    QDate day() const;
    void setDay(QDate day);

    int difference() const;

signals:
    void personIdChanged(int personId);
    void dayStartsChanged(QTime dayStarts);
    void dayChanged(QDate day);
    void differenceChanged(int difference);

private:
    void setDifference(int difference);

    std::vector<Liquid> m_liquids;
    QTime m_dayStarts = QTime(7,0);
    QDate m_day;
    int m_personId = 0;
    int m_difference = 0;
};

#endif // LIQUIDLISTMODEL_H
