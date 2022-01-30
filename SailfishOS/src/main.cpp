/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QGuiApplication>
#include <QQuickView>
#include <QLocale>
#include <QTranslator>
#include <QtQml>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QDir>

#include <sailfishapp.h>
#include <memory>

#include <hbnsc.h>
#include <hbnsciconprovider.h>
#include <hbnsclicensemodel.h>

#include "sfosconfig.h"
#include "models/licensesmodel.h"

#include "../../common/src/objects/person.h"
#include "../../common/src/models/peoplelistmodel.h"
#include "../../common/src/models/languagesmodel.h"
#include "../../common/src/migrations/m20220127t134808_people.h"
#include "../../common/src/migrations/m20220130t123658_liquid.h"

int main(int argc, char *argv[])
{
    std::unique_ptr<QGuiApplication> app(SailfishApp::application(argc, argv));
    QCoreApplication::setApplicationName(QStringLiteral("harbour-nazzida"));
    QGuiApplication::setApplicationDisplayName(QStringLiteral("Nazzida"));
    QCoreApplication::setApplicationVersion(QStringLiteral(NAZZIDA_VERSION));

    auto config = new SfosConfig(app.get());

    if (!config->language().isEmpty()) {
        QLocale::setDefault(QLocale(config->language()));
    }

    {
        const QLocale locale;
        for (const QString &name : {QStringLiteral("hbnsc"), QStringLiteral("nazzida")}) {
            auto trans = new QTranslator(app.get());
            if (Q_LIKELY(trans->load(locale, name, QStringLiteral("_"), QStringLiteral(NAZZIDA_I18NDIR), QStringLiteral(".qm")))) {
                if (Q_UNLIKELY(!app->installTranslator(trans))) {
                    qWarning(R"(Can not install translator for component "%s" and locale "%s".)", qUtf8Printable(name), qUtf8Printable(locale.name()));
                }
            } else {
                qWarning(R"(Can not load translations for component "%s" and locale "%s".)", qUtf8Printable(name), qUtf8Printable(locale.name()));
            }
        }
    }

    QString errorMessage;
    QString dbFile;

    {
        QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));

        if (Q_UNLIKELY(!dataDir.exists())) {
            if (!dataDir.mkpath(dataDir.absolutePath())) {
                //: error message, %1 will be the directory path
                //% "Failed to create the data directory %1"
                errorMessage = qtTrId("naz-err-failed-create-data-dir").arg(dataDir.absolutePath());
                qCritical("Failed to create the data directory %s", qUtf8Printable(dataDir.absolutePath()));
            }
        }

        if (errorMessage.isEmpty()) {
            dbFile = dataDir.absoluteFilePath(QStringLiteral("nazzida.sqlite"));

            QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("initDbCon"));
            db.setDatabaseName(dbFile);

            if (!db.open()) {
                //: error message, %1 will be the path to the database file, %2
                //: will be the database error message
                //% "Failed to open database %1: %2"
                errorMessage = qtTrId("naz-err-failed-open-db").arg(dbFile, db.lastError().text());
                qCritical("Failed to open database %s: %s", qUtf8Printable(dbFile), qUtf8Printable(db.lastError().text()));
            }

            if (errorMessage.isEmpty()) {
                QSqlQuery q(db);
                if (!q.exec(QStringLiteral("PRAGMA foreign_keys = ON"))) {
                    //: error message, %1 will be the database error message
                    //% "Failed to enable foreign keys pragma: %1"
                    errorMessage = qtTrId("naz-err-failed-foreign-keys-pragma").arg(q.lastError().text());
                    qCritical("Failed to enable foreign keys pragma: %s", qUtf8Printable(q.lastError().text()));
                }
            }

            if (errorMessage.isEmpty()) {
                auto migrator = new Firfuorida::Migrator(QStringLiteral("initDbCon"), QStringLiteral("migrations"), app.get());
                new M20220127T134808_People(migrator);
                new M20220130T123658_Liquid(migrator);

                if (!migrator->migrate()) {
                    //: error message, %1 will be replaced by the migration error
                    //% "Failed to perform database migrations: %1"
                    errorMessage = qtTrId("naz-err-failed-db-migrations").arg(migrator->lastError().text());
                    qCritical("%s", "Failed to perform database migrations.");
                }
            }

            db.close();
        }

        QSqlDatabase::removeDatabase(QStringLiteral("initDbCon"));
    }

    {
        QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
        db.setDatabaseName(dbFile);
        if (!db.open()) {
            errorMessage = qtTrId("naz-err-failed-open-db").arg(dbFile, db.lastError().text());
            qCritical("Failed to open database %s: %s", qUtf8Printable(dbFile), qUtf8Printable(db.lastError().text()));
        }

        if (errorMessage.isEmpty()) {
            QSqlQuery q(db);
            if (!q.exec(QStringLiteral("PRAGMA foreign_keys = ON"))) {
                errorMessage = qtTrId("naz-err-failed-foreign-keys-pragma").arg(q.lastError().text());
                qCritical("Failed to enable foreign keys pragma: %s", qUtf8Printable(q.lastError().text()));
            }
        }
    }

    qmlRegisterType<LicensesModel>("harbour.nazzida", 1, 0, "LicensesModel");
    qmlRegisterType<Person>("harbour.nazzida", 1, 0, "Person");
    qmlRegisterType<PeopleListModel>("harbour.nazzida", 1, 0, "PeopleListModel");
    qmlRegisterType<LanguagesModel>("harbour.nazzida", 1, 0, "LanguagesModel");

    std::unique_ptr<QQuickView> view(SailfishApp::createView());

    auto hbnscIconProvider = Hbnsc::HbnscIconProvider::createProvider(view->engine());
    auto nazzidaIconProvider = Hbnsc::BaseIconProvider::createProvider({1.0,1.25,1.5,1.75,2.0}, QString(), false, QStringLiteral("nazzida"), view->engine());

    view->rootContext()->setContextProperty(QStringLiteral("config"), config);
    view->rootContext()->setContextProperty(QStringLiteral("startupError"), errorMessage);

    view->setSource(SailfishApp::pathToMainQml());

    view->showFullScreen();

    return app->exec();
}
