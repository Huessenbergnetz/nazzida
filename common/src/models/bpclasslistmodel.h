/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef BPCLASSLISTMODEL_H
#define BPCLASSLISTMODEL_H

#include <QAbstractListModel>
#include <QUrl>
#include "configuration.h"

class BpClassListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit BpClassListModel(QObject *parent = nullptr);
    ~BpClassListModel() override;

    enum Roles {
        Value = Qt::UserRole +1,
        Name,
        Description,
        Year,
        Url
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const final;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const final;
    QHash<int, QByteArray> roleNames() const final;
    QVariant data(const QModelIndex &index, int role = Qt::UserRole) const final;

    Q_INVOKABLE int findIndex(int value) const;

private:
    struct BpClass {
        QString name;
        QString description;
        QUrl url;
        int year;
        Configuration::BpClass value{Configuration::EshIsh};

        BpClass(const QString &_name, const QString &_description, const QUrl &_url, int _year, Configuration::BpClass _value)
            : name{_name}
            , description{_description}
            , url{_url}
            , year{_year}
            , value{_value}
        {}

        BpClass(const BpClass &) = default;
        BpClass(BpClass &&) noexcept = default;
        BpClass& operator=(const BpClass &) = default;
        BpClass& operator=(BpClass &&) noexcept = default;
        ~BpClass() noexcept = default;

        void swap(BpClass &other)
        {
            std::swap(name, other.name);
            std::swap(description, other.description);
            std::swap(url, other.url);
            std::swap(year, other.year);
            std::swap(value, other.value);
        }
    };

    void populate();

    std::vector<BpClass> m_bpClasses;

    Q_DISABLE_COPY(BpClassListModel)
};

#endif // BPCLASSLISTMODEL_H
