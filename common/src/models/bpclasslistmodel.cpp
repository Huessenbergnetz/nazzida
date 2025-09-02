/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "bpclasslistmodel.h"

BpClassListModel::BpClassListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    populate();
}

BpClassListModel::~BpClassListModel() = default;

int BpClassListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_bpClasses.size();
}

QModelIndex BpClassListModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    return createIndex(row, column);
}

QHash<int, QByteArray> BpClassListModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(Value, QByteArrayLiteral("value"));
    roles.insert(Name, QByteArrayLiteral("name"));
    roles.insert(Description, QByteArrayLiteral("description"));
    roles.insert(Url, QByteArray("url"));
    roles.insert(Year, QByteArrayLiteral("year"));
    return roles;
}


QVariant BpClassListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() > (rowCount() -1 )) {
        return QVariant();
    }

    const BpClass bpc = m_bpClasses.at(index.row());

    switch(role) {
    case Value:
        return bpc.value;
    case Name:
        return bpc.name;
    case Description:
        return bpc.description;
    case Year:
        return bpc.year;
    case Url:
        return bpc.url;
    }

    return QVariant();
}

int BpClassListModel::findIndex(int value) const
{
    if (rowCount() == 0) {
        return -1;
    }

    int idx             = -1;
    const auto _value   = static_cast<Configuration::BpClass>(value);
    const auto size     = static_cast<int>(m_bpClasses.size());

    for (int i = 0; i < size; ++i) {
        if (m_bpClasses.at(i).value == _value) {
            idx = i;
            break;
        }
    }

    return idx;
}

void BpClassListModel::populate()
{
    m_bpClasses.reserve(3);

    m_bpClasses.emplace_back(QStringLiteral("ESC"),
                             //% "Blood pressure classification according to the European Society of Cardiology"
                             qtTrId("naz-bpclass-esc-desc"),
                             QUrl(QStringLiteral("https://academic.oup.com/eurheartj/article/45/38/3912/7741010")),
                             2024,
                             Configuration::Esc);

    m_bpClasses.emplace_back(QStringLiteral("ESH/ISH"),
                             //% "Blood pressure classification according to the European Society of Hypertension/"
                             //% "International Society of Hypertension"
                             qtTrId("naz-bpclass-eshish-desc"),
                             QUrl(QStringLiteral("")),
                             2023,
                             Configuration::EshIsh);

    m_bpClasses.emplace_back(QStringLiteral("ACC/AHH"),
                             //% "Blood pressure classification according to the American College of Cardiology/"
                             //% "American Heart Association"
                             qtTrId("naz-bpclass-accahh-desc"),
                             QUrl(QStringLiteral("https://pmc.ncbi.nlm.nih.gov/articles/PMC9470378/")),
                             2017,
                             Configuration::AccAhh);
}

#include "moc_bpclasslistmodel.cpp"
