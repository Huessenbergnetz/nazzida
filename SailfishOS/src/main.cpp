/*
 * SPDX-FileCopyrightText: (C) 2022-2025 Matthias Fehring / www.huessenbergnetz.de
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
#include <QDebug>

#include <sailfishapp.h>
#include <memory>
#include <vector>
#include <utility>

#include <hbnsc.h>
#include <hbnsciconprovider.h>
#include <hbnsclicensemodel.h>

#include "sfosconfig.h"
#include "nazzidautils.h"
#include "models/licensesmodel.h"
#include "models/languagesmodel.h"
#include "models/bpclasslistmodel.h"

#include "objects/person.h"
#include "models/peoplelistfiltermodel.h"
#include "objects/dailyliquids.h"
#include "models/dailyliquidlistfiltermodel.h"
#include "objects/liquid.h"
#include "models/liquidlistfiltermodel.h"
#include "objects/weight.h"
#include "models/weightlistfiltermodel.h"
#include "objects/bloodpressure.h"
#include "models/bloodpressurelistfiltermodel.h"

#include "migrations/m20220127t134808_people.h"
#include "migrations/m20220130t123658_liquid.h"
#include "migrations/m20220218t081651_people_transpire.h"
#include "migrations/m20250830t120537_weight.h"
#include "migrations/m20250901t134046_sysinfo.h"
#include "migrations/m20250901t151150_bloodpressure.h"

int main(int argc, char *argv[])
{
    std::unique_ptr<QGuiApplication> app(SailfishApp::application(argc, argv));
    app->setApplicationName(QStringLiteral("harbour-nazzida"));
    app->setApplicationDisplayName(QStringLiteral("Nazzida"));
    app->setOrganizationName(QStringLiteral("de.huessenbergnetz"));
    app->setApplicationVersion(QStringLiteral(NAZZIDA_VERSION));

    auto config = new SfosConfig(app.get());

    if (!config->language().isEmpty()) {
        QLocale::setDefault(QLocale(config->language()));
    }

    {
        const QLocale locale;
        qDebug("Loading translations for locale %s", qUtf8Printable(locale.name()));
        for (const QString &name : {QStringLiteral("hbnsc"), QStringLiteral("nazzida")}) {
            auto trans = new QTranslator(app.get());
            if (Q_LIKELY(trans->load(locale, name, QStringLiteral("_"), QStringLiteral(NAZZIDA_I18NDIR), QStringLiteral(".qm")))) {
                if (Q_UNLIKELY(!app->installTranslator(trans))) {
                    qWarning(R"(Can not install translator for component "%s" and locale "%s".)", qUtf8Printable(name), qUtf8Printable(locale.name()));
                } else {
                    qDebug("Loaded translations for %s", qUtf8Printable(name));
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

            qDebug("Initializing database %s", qUtf8Printable(dbFile));

            if (!db.open()) {
                //: error message, %1 will be the path to the database file, %2
                //: will be the database error message
                //% "Failed to open database %1: %2"
                errorMessage = qtTrId("naz-err-failed-open-db").arg(dbFile, db.lastError().text());
                qCritical("Failed to open database %s: %s", qUtf8Printable(dbFile), qUtf8Printable(db.lastError().text()));
            }

            if (errorMessage.isEmpty()) {
                qDebug("%s", "Enabling foreign_keys pragma");
                QSqlQuery q(db);
                if (!q.exec(QStringLiteral("PRAGMA foreign_keys = ON"))) {
                    //: error message, %1 will be the database error message
                    //% "Failed to enable foreign keys pragma: %1"
                    errorMessage = qtTrId("naz-err-failed-foreign-keys-pragma").arg(q.lastError().text());
                    qCritical("Failed to enable foreign keys pragma: %s", qUtf8Printable(q.lastError().text()));
                }
            }

            if (errorMessage.isEmpty()) {
                auto migrator = std::make_unique<Firfuorida::Migrator>(QStringLiteral("initDbCon"), QStringLiteral("migrations"), app.get());
                new M20220127T134808_People(migrator.get());
                new M20220130T123658_Liquid(migrator.get());
                new M20220218T081651_People_transpire(migrator.get());
                new M20250830T120537_Weight(migrator.get());
                new M20250901T134046_SysInfo(migrator.get());
                new M20250901T151150_BloodPressure(migrator.get());

                if (!migrator->migrate()) {
                    //: error message, %1 will be replaced by the migration error
                    //% "Failed to perform database migrations: %1"
                    errorMessage = qtTrId("naz-err-failed-db-migrations").arg(migrator->lastError().text());
                    qCritical("%s", "Failed to perform database migrations.");
                }
            }

            if (errorMessage.isEmpty()) {
                QSqlQuery q(db);
                if (!q.exec(QStringLiteral("SELECT value FROM sysinfo WHERE name = 'utctimes'"))) {
                    //: error message, % 1 will be the database error message
                    //% "Failed to query “sysinfo” table for “utctimes“ entry: %1"
                    errorMessage = qtTrId("naz-err-failed-get-utctimes-sysinfo").arg(q.lastError().text());
                    qCritical() << "Failed to query sysinfo table for utctimes enty:" << q.lastError().text();
                } else {
                    bool utctimes = false;
                    if (q.next()) {
                        utctimes = q.value(0).toInt() == 1;
                    }
                    if (!utctimes) {
                        qInfo() << "Start converting datetimes in database to UTC";
                        for (const QString &table : {QStringLiteral("liquid"), QStringLiteral("weight")}) {
                            q.exec(QStringLiteral("SELECT id, moment FROM %1").arg(table));

                            std::vector<std::pair<int, QDateTime>> data;

                            while (q.next()) {
                                data.emplace_back(q.value(0).toInt(), q.value(1).toDateTime());
                            }

                            if (!data.empty()) {
                                q.prepare(QStringLiteral("UPDATE %1 SET moment = :moment WHERE id = :id").arg(table));
                                for (const auto &p : data) {
                                    qDebug() << "Converting" << table << "ID" << p.first << "moment from" << p.second << "to" << p.second.toUTC();
                                    q.bindValue(":moment", p.second.toUTC());
                                    q.bindValue(":id", p.first);
                                    if (!q.exec()) {
                                        qCritical() << "Failed to convert datetimes for table" << table << "with error:" << q.lastError().text();
                                    }
                                }
                            }
                        }
                        q.exec("INSERT INTO sysinfo (name, value) VALUES ('utctimes', '1')");
                        qInfo() << "Finished converting datetimes in database to UTC";
                    }
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
            errorMessage = qtTrId("naz-err-failed-open-db").arg(dbFile, db.lastError().text());
            qCritical("Failed to open database %s: %s", qUtf8Printable(dbFile), qUtf8Printable(db.lastError().text()));
        }

        if (errorMessage.isEmpty()) {
            qDebug("%s", "Enabling foreign_keys pragma");
            QSqlQuery q(db);
            if (!q.exec(QStringLiteral("PRAGMA foreign_keys = ON"))) {
                errorMessage = qtTrId("naz-err-failed-foreign-keys-pragma").arg(q.lastError().text());
                qCritical("Failed to enable foreign keys pragma: %s", qUtf8Printable(q.lastError().text()));
            }
        }
    }

    qmlRegisterType<LicensesModel>("harbour.nazzida", 1, 0, "LicensesModel");
    qmlRegisterType<Person>("harbour.nazzida", 1, 0, "Person");
    qmlRegisterType<PeopleListFilterModel>("harbour.nazzida", 1, 0, "PeopleListFilterModel");
    qmlRegisterUncreatableType<DailyLiquids>("harbour.nazzida", 1, 0, "DailyLiquids", QStringLiteral("You can not create objects of type DailyLiquids in QML!"));
    qmlRegisterType<DailyLiquidListFilterModel>("harbour.nazzida", 1, 0, "DailyLiquidListFilterModel");
    qmlRegisterUncreatableType<Liquid>("harbour.nazzida", 1, 0, "Liquid", QStringLiteral("You can not create objects of type Liquid in QML!"));
    qmlRegisterType<LiquidListFilterModel>("harbour.nazzida", 1, 0, "LiquidListFilterModel");
    qmlRegisterUncreatableType<Weight>("harbour.nazzida", 1, 0, "Weight", QStringLiteral("You can not create object of type Weight in QML!"));
    qmlRegisterType<WeightListFilterModel>("harbour.nazzida", 1, 0, "WeightListFilterModel");
    qmlRegisterUncreatableType<BloodPressure>("harbour.nazzida", 1, 0, "BloodPressure", QStringLiteral("You can not create object of type BloodPressure in QML!"));
    qmlRegisterType<BloodPressureListFilterModel>("harbour.nazzida", 1, 0, "BloodPressureListFilterModel");
    qmlRegisterType<LanguagesModel>("harbour.nazzida", 1, 0, "LanguagesModel");
    qmlRegisterSingletonType<NazzidaUtils>("harbour.nazzida", 1, 0, "NazzidaUtils", NazzidaUtils::provider);
    qmlRegisterType<BpClassListModel>("harbour.nazzida", 1, 0, "BpClassListModel");

    std::unique_ptr<QQuickView> view(SailfishApp::createView());

    Hbnsc::HbnscIconProvider::addProvider(view->engine());
    Hbnsc::BaseIconProvider::addProvider(view->engine(), QStringLiteral("nazzida"), {1.0,1.25,1.5,1.75,2.0});

    view->rootContext()->setContextProperty(QStringLiteral("config"), config);
    view->rootContext()->setContextProperty(QStringLiteral("startupError"), errorMessage);
    view->rootContext()->setContextProperty(QStringLiteral("coverIconPath"), Hbnsc::getLauncherIcon({86,108,128,150,172}));

    view->setSource(SailfishApp::pathToMainQml());

    view->showFullScreen();

    return app->exec();
}
