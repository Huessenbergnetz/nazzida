/*
 * SPDX-FileCopyrightText: (C) 2022-2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef BASEFILTERMODEL_H
#define BASEFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <QObject>

class BaseListModel;

class BaseFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool inOperation READ inOperation NOTIFY inOperationChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortOrderChanged)
public:
    explicit BaseFilterModel(QObject *parent = nullptr);
    ~BaseFilterModel() override;

    virtual bool inOperation() const;

    virtual QString lastError() const;

    Qt::SortOrder sortOrder() const;
    void setSortOrder(Qt::SortOrder order);

    Q_INVOKABLE bool load();
    Q_INVOKABLE void clear();

signals:
    void inOperationChanged(bool inOperation);
    void lastErrorChanged(const QString &lastError);
    void sortOrderChanged(Qt::SortOrder sortOrder);

protected:
    void setModel(BaseListModel *model);
    BaseListModel* model() const;

private:
    Q_DISABLE_COPY(BaseFilterModel)

    BaseListModel *m_model = nullptr;
    Qt::SortOrder m_sortOrder = Qt::AscendingOrder;
};

#endif // BASEFILTERMODEL_H
