/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
#include <KLocalizedContext>
#include <KLocalizedString>

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

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));

    engine.load(QStringLiteral(NAZZIDA_QML_DIR) + QStringLiteral("/main.qml"));

    return app.exec();
}

