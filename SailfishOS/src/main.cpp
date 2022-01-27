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
#include <QStandardPaths>
#include <QDir>

#include <sailfishapp.h>
#include <memory>

#include <hbnsc.h>
#include <hbnsciconprovider.h>
#include <hbnsclicensemodel.h>

#include "sfosconfig.h"
#include "models/licensesmodel.h"

#include "../../common/src/models/peoplelistmodel.h"
#include "../../common/src/migrations/m20220127t134808_people.h"

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

            QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("intDbCon"));
            db.setDatabaseName(dbFile);

            if (!db.open()) {
                //: error message, %1 will be the path to the database file, %2
                //: will be the database error message
                //% "Failed to open database %1: %2"
                errorMessage = qtTrId("naz-err-failed-open-db").arg(dbFile, db.lastError().text());
                qCritical("Failed to open database %s: %s", qUtf8Printable(dbFile), qUtf8Printable(db.lastError().text()));
            }

            if (errorMessage.isEmpty()) {
                auto migrator = new Firfuorida::Migrator(QStringLiteral("intDbCon"), QStringLiteral("migrations"), app.get());
                new M20220127T134808_People(migrator);

                if (!migrator->migrate()) {
                    //: error message, %1 will be replaced by the migration error
                    //% "Failed to perform database migrations: %1"
                    errorMessage = qtTrId("naz-err-failed-db-migrations").arg(migrator->lastError().text());
                    qCritical("%s", "Failed to perform database migrations.");
                }
            }

            db.close();
        }

        QSqlDatabase::removeDatabase(QStringLiteral("intDbCon"));
    }

    {
        QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
        db.setDatabaseName(dbFile);
        if (!db.open()) {
            errorMessage = qtTrId("naz-err-failed-open-db").arg(dbFile, db.lastError().text());
            qCritical("Failed to open database %s: %s", qUtf8Printable(dbFile), qUtf8Printable(db.lastError().text()));
        }
    }

    qmlRegisterType<LicensesModel>("harbour.nazzida", 1, 0, "LicensesModel");
    qmlRegisterType<PeopleListModel>("harbour.nazzida", 1, 0, "PeopleListModel");

    std::unique_ptr<QQuickView> view(SailfishApp::createView());

    auto hbnscIconProvider = Hbnsc::HbnscIconProvider::createProvider(view->engine());

    view->rootContext()->setContextProperty(QStringLiteral("config"), config);
    view->rootContext()->setContextProperty(QStringLiteral("startupError"), errorMessage);

    view->setSource(SailfishApp::pathToMainQml());

    view->showFullScreen();

    return app->exec();
}
