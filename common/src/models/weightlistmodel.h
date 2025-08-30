/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef WEIGHTLISTMODEL_H
#define WEIGHTLISTMODEL_H

#include "baselistmodel.h"
#include "../objects/weight.h"
#include <QObject>
#include <vector>

class WeightListModel : public BaseListModel
{
    Q_OBJECT
    Q_PROPERTY(int personId READ personId WRITE setPersonId NOTIFY personIdChanged)
public:
    explicit WeightListModel(QObject *parent = nullptr);
    ~WeightListModel() override;

    enum Roles : int {
        Id = Qt::UserRole + 1,
        PersonId,
        Moment,
        WeightValue,
        Note
    };
    Q_ENUM(Roles)

    Q_INVOKABLE bool load() override;
    Q_INVOKABLE bool loadForPerson(int personId);
    Q_INVOKABLE void clear() override;

    Q_INVOKABLE bool add(const QDateTime &moment, int weight, const QString &note);
    Q_INVOKABLE bool remove(QModelIndex index);
    Q_INVOKABLE bool edit(QModelIndex index, const QDateTime &moment, int weight, const QString &note);

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
    std::vector<Weight> m_weights;
    int m_personId{0};
};

#endif // WEIGHTLISTMODEL_H
