/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include "updater.h"

#include <QDateTime>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <utility>
#include <vector>

Updater::Updater(QObject *parent) : QObject(parent)
{

}

bool Updater::checkUpdatesRequired(const QString &dbCon, QString &errorString)
{
    auto db = QSqlDatabase::database(dbCon);

    if (!db.open()) {
        qCritical().noquote().nospace()
                << "Failed to open SQLite database "
                << db.databaseName() << ": " << db.lastError();
        // defined in main.cpp
        errorString = qtTrId("naz-err-failed-open-db").arg(db.databaseName(), db.lastError().text());;
        return false;
    }

    QSqlQuery q(db);

    // start checking if datetimes are stored as UTC

    if (Q_UNLIKELY(!q.exec(QStringLiteral("SELECT value FROM sysinfo WHERE name = 'utctimes'")))) {
        qCritical() << "Failed to query sysinfo table for utctimes entry:" << q.lastError();
        //: error message, % 1 will be the database error message
        //% "Failed to query “sysinfo” table for “utctimes“ entry: %1"
        errorString = qtTrId("naz-err-failed-get-utctimes-sysinfo").arg(q.lastError().text());
        return false;
    }

    if (!q.next()) {
        return true;
    }

    if (q.value(0).toInt() != 1) {
        return true;
    }

    // end checking if datetimes are stored as UTC

    return false;
}

bool Updater::inOperation() const
{
    return m_inOperation;
}

void Updater::setInOperation(bool inOperation)
{
    if (m_inOperation != inOperation) {
        m_inOperation = inOperation;
        emit inOperationChanged(this->inOperation());
    }
}

bool Updater::hasFinished() const
{
    return m_hasFinished;
}

void Updater::setHasFinished(bool hasFinished)
{
    if (m_hasFinished != hasFinished) {
        m_hasFinished = hasFinished;
        emit hasFinishedChanged(this->hasFinished());
        setInOperation(false);
    }
}

QString Updater::currentAction() const
{
    return m_currentAction;
}

void Updater::setCurrentAction(const QString &currentAction)
{
    if (m_currentAction != currentAction) {
        m_currentAction = currentAction;
        emit currentActionChanged(this->currentAction());
    }
}

bool Updater::hasError() const
{
    return !m_error.isEmpty();
}

QString Updater::error() const
{
    return m_error;
}

void Updater::setError(const QString &error)
{
    if (m_error != error) {
        m_error = error;
        emit hasErrorChanged(this->hasError());
        emit errorChanged(this->error());
        if (!m_error.isEmpty()) {
            setHasFinished(true);
        }
    }
}

void Updater::startUpdate()
{
    setInOperation(true);

    QSqlQuery q;

    // start updating datetimes to utc

    //: action description while updating database
    //% "Updating datetimes in database to UTC"
    setCurrentAction(qtTrId("naz-updater-action-update-datetimes"));
    qDebug() << "Start converting datetimes in database to UTC";

    for (const QString &table : {QStringLiteral("liquid"), QStringLiteral("weight")}) {

        if (Q_UNLIKELY(!q.exec(QStringLiteral("SELECT id, moment FROM %1").arg(table)))) {
            //: error message, %1 will be replaced by the db error string
            //% "Failed to execute database query: %1"
            setError(qtTrId("naz-updater-error-failed-db-exec").arg(q.lastError().text()));
            qCritical() << "Failed to execute database query:" << q.lastError();
            return;
        }

        std::vector<std::pair<int, QDateTime>> data;

        while (q.next()) {
            data.emplace_back(q.value(0).toInt(), q.value(1).toDateTime());
        }

        if (!data.empty()) {
            if (Q_UNLIKELY(!q.prepare(QStringLiteral("UPDATE %1 SET moment = :moment WHERE id = :id").arg(table)))) {
                //% "Failed to prepare database query: %1"
                setError(qtTrId("naz-updater-error-failed-db-prepare").arg(q.lastError().text()));
                qCritical() << "Failed to prepare database query:" << q.lastError();
                return;
            }
            for (const auto &p : data) {
                qDebug() << "Converting" << table << "ID" << p.first << "moment from" << p.second << "to" << p.second.toUTC();
                q.bindValue(QStringLiteral(":moment"), p.second.toUTC());
                q.bindValue(QStringLiteral(":id"), p.first);
                if (Q_UNLIKELY(!q.exec())) {
                    setError(qtTrId("naz-updater-error-failed-db-exec").arg(q.lastError().text()));
                    qCritical() << "Failed to execute database query:" << q.lastError();
                    return;
                }
            }
        }
    }

    if (Q_UNLIKELY(!q.exec(QStringLiteral("INSERT INTO sysinfo (name, value) VALUES ('utctimes', '1')")))) {
        setError(qtTrId("naz-updater-error-failed-db-exec").arg(q.lastError().text()));
        qCritical() << "Failed to execute database query:" << q.lastError();
        return;
    }

    qDebug() << "Finished converting datetimes in database to UTC";

    // finished update datetimes to utc

    setHasFinished(true);
}

#include "moc_updater.cpp"
