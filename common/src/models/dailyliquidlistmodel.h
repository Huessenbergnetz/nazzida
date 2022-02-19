/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef DAILYLIQUIDLISTMODEL_H
#define DAILYLIQUIDLISTMODEL_H

#include "baselistmodel.h"
#include "../objects/dailyliquids.h"
#include <QObject>
#include <QTime>
#include <vector>

class DailyLiquidListModel : public BaseListModel
{
    Q_OBJECT
    Q_PROPERTY(int personId READ personId WRITE setPersonId NOTIFY personIdChanged)
    Q_PROPERTY(QTime dayStarts READ dayStarts WRITE setDayStarts NOTIFY dayStartsChanged)
public:
    explicit DailyLiquidListModel(QObject *parent = nullptr);
    ~DailyLiquidListModel() override;

    enum Roles {
        Id = Qt::UserRole + 1,
        PersonId,
        Day,
        Input,
        Output,
        Difference,
        Entries
    };
    Q_ENUM(Roles)

    Q_INVOKABLE bool load() override;
    Q_INVOKABLE bool loadForPerson(int personId, QTime dayStarts);
    Q_INVOKABLE void clear() override;

    Q_INVOKABLE int add(const QDateTime &moment, int inOrOut, int amount, const QString &name, const QString &note);
    Q_INVOKABLE bool remove(const QDateTime &moment, int inOrOut, int amount);
    Q_INVOKABLE bool edit(const QDateTime &oldMoment, const QDateTime &newMoment, int oldInOrOut, int newInOrOut, int oldAmount, int newAmount);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

    int personId() const;
    void setPersonId(int id);

    QTime dayStarts() const;
    void setDayStarts(QTime dayStarts);

signals:
    void personIdChanged(int personId);
    void dayStartsChanged(QTime dayStarts);

private:
    std::vector<DailyLiquids> m_dailyLiquids;
    QTime m_dayStarts = QTime(7, 0);
    int m_personId = 0;
};

#endif // DAILYLIQUIDLISTMODEL_H
