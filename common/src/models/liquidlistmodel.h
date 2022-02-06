/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef LIQUIDLISTMODEL_H
#define LIQUIDLISTMODEL_H

#include "baselistmodel.h"
#include "../objects/liquid.h"
#include <QObject>
#include <vector>

class LiquidListModel : public BaseListModel
{
    Q_OBJECT
    Q_PROPERTY(int personId READ personId WRITE setPersonId NOTIFY personIdChanged)
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
    Q_INVOKABLE bool loadForPerson(int personId);
    Q_INVOKABLE void clear() override;

    Q_INVOKABLE int add(const QDateTime &moment, int inOrOut, int amount, const QString &name, const QString &note);
    Q_INVOKABLE bool remove(QModelIndex index);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    int personId() const;
    void setPersonId(int id);

signals:
    void personIdChanged(int personId);

private:
    std::vector<Liquid> m_liquids;
    int m_personId = 0;
};

#endif // LIQUIDLISTMODEL_H
