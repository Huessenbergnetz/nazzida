/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
#include <QDir>
#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "../../common/src/migrations/m20220127t134808_people.h"
#include "../../common/src/migrations/m20220130t123658_liquid.h"
#include "../../common/src/migrations/m20220218t081651_people_transpire.h"

#include "../../common/src/objects/person.h"
#include "../../common/src/models/peoplelistfiltermodel.h"
#include "../../common/src/objects/liquid.h"
#include "../../common/src/models/liquidlistfiltermodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("nazzida");
    QCoreApplication::setOrganizationName(QStringLiteral("Huessenbergnetz"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("huessenbergnetz.de"));
    QCoreApplication::setApplicationName(QStringLiteral("nazzida"));
    QGuiApplication::setApplicationDisplayName(QStringLiteral("Nazzida"));
    QCoreApplication::setApplicationVersion(QStringLiteral(NAZZIDA_VERSION));

    QString errorMessage;
    QString dbFile;

    {
        QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));

        if (Q_UNLIKELY(!dataDir.exists())) {
            if (!dataDir.mkpath(dataDir.absolutePath())) {
                errorMessage = i18nc("error message, %1 will be the directory path", "Failed to create the data directory %1", dataDir.absolutePath());
                qCritical("Failed to create the data directory %s", qUtf8Printable(dataDir.absolutePath()));
            }
        }

        if (errorMessage.isEmpty()) {
            dbFile = dataDir.absoluteFilePath(QStringLiteral("nazzida.sqlite"));

            QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("initDbCon"));
            db.setDatabaseName(dbFile);

            qDebug("Initializing database %s", qUtf8Printable(dbFile));

            if (!db.open()) {
                errorMessage = i18nc("error message, %1 is db path, %2 db error", "Failed to open database %1: %2", dbFile, db.lastError().text());
                qCritical("Failed to open database %s: %s", qUtf8Printable(dbFile), qUtf8Printable(db.lastError().text()));
            }

            if (errorMessage.isEmpty()) {
                qDebug("%s", "Enabling foreign_keys pragma");
                QSqlQuery q(db);
                if (!q.exec(QStringLiteral("PRAGMA foreign_keys = ON"))) {
                    errorMessage = i18nc("error message, %1 is db error", "Failed to enable foreign keys pragma: %1", q.lastError().text());
                    qCritical("Failed to enable foreign keys pragma: %s", qUtf8Printable(q.lastError().text()));
                }
            }

            if (errorMessage.isEmpty()) {
                auto migrator = new Firfuorida::Migrator(QStringLiteral("initDbCon"), QStringLiteral("migrations"), &app);
                new M20220127T134808_People(migrator);
                new M20220130T123658_Liquid(migrator);
                new M20220218T081651_People_transpire(migrator);

                if (!migrator->migrate()) {
                    errorMessage = i18nc("error message, %1 is the migration error", "Failed to perform database migrations: %1").arg(migrator->lastError().text());
                    qCritical("Failed to perform database migrations: %s", qUtf8Printable(migrator->lastError().text()));
                }
            }

            db.close();
        }

        QSqlDatabase::removeDatabase(QStringLiteral("initDbCon"));
    }

    {
        qDebug("Opening database %s", qUtf8Printable(dbFile));
        QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
        db.setDatabaseName(dbFile);
        if (!db.open()) {
            errorMessage = i18nc("error message, %1 is db path, %2 db error", "Failed to open database %1: %2", dbFile, db.lastError().text());
            qCritical("Failed to open database %s: %s", qUtf8Printable(dbFile), qUtf8Printable(db.lastError().text()));
        }

        if (errorMessage.isEmpty()) {
            qDebug("%s", "Enabling foreign_keys pragma");
            QSqlQuery q(db);
            if (!q.exec(QStringLiteral("PRAGMA foreign_keys = ON"))) {
                errorMessage = i18nc("error message, %1 is db error", "Failed to enable foreign keys pragma: %1", q.lastError().text());
                qCritical("Failed to enable foreign keys pragma: %s", qUtf8Printable(q.lastError().text()));
            }
        }
    }

    qmlRegisterType<Person>("de.huessenbergnetz.nazzida", 1, 0, "Person");
    qmlRegisterType<PeopleListFilterModel>("de.huessenbergnetz.nazzida", 1, 0, "PeopleListFilterModel");
    qmlRegisterUncreatableType<Liquid>("de.huessenbergnetz.nazzida", 1, 0, "Liquid", QStringLiteral("You can not create objects of type Liquid in QML!"));
    qmlRegisterType<LiquidListFilterModel>("de.huessenbergnetz.nazzida", 1, 0, "LiquidListFilterModel");

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));

    engine.load(QStringLiteral(NAZZIDA_QML_DIR) + QStringLiteral("/main.qml"));

    return app.exec();
}

