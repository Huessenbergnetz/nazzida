/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>

class Updater : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool inOperation READ inOperation NOTIFY inOperationChanged)
    Q_PROPERTY(bool hasFinished READ hasFinished NOTIFY hasFinishedChanged)
    Q_PROPERTY(QString currentAction READ currentAction NOTIFY currentActionChanged)
    Q_PROPERTY(bool hasError READ hasError NOTIFY hasErrorChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)
public:
    explicit Updater(QObject *parent = nullptr);
    ~Updater() override = default;

    bool inOperation() const;
    bool hasFinished() const;
    bool hasError() const;
    QString currentAction() const;
    QString error() const;

    Q_INVOKABLE void startUpdate();

    static bool checkUpdatesRequired(const QString &dbCon, QString &errorString);

signals:
    void inOperationChanged(bool inOperation);
    void hasFinishedChanged(bool hasFinished);
    void hasErrorChanged(bool hasError);
    void currentActionChanged(const QString &currentAction);
    void errorChanged(const QString &error);

private:
    void setInOperation(bool inOperation);
    void setHasFinished(bool hasFinished);
    void setCurrentAction(const QString &currentAction);
    void setError(const QString &error);

    QString m_currentAction;
    QString m_error;
    bool m_inOperation{false};
    bool m_hasFinished{false};
};

#endif // UPDATER_H
