/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef BASELISTMODEL_H
#define BASELISTMODEL_H

#include <QAbstractListModel>
#include <QObject>

class BaseListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool inOperation READ inOperation NOTIFY inOperationChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)
public:
    explicit BaseListModel(QObject *parent = nullptr);
    ~BaseListModel() override;

    Q_INVOKABLE virtual bool load();
    Q_INVOKABLE virtual void clear();

    bool inOperation() const;

    QString lastError() const;

protected:
    void setInOperation(bool inOp);
    void setLastError(const QString &error);

signals:
    void inOperationChanged(bool inOperation);
    void lastErrorChanged(const QString &lastError);

private:
    QString m_lastError;
    bool m_inOperation = false;
};

#endif // BASELISTMODEL_H
