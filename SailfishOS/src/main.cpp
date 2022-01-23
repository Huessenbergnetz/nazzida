/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QGuiApplication>
#include <QQuickView>

#include <sailfishapp.h>
#include <memory>

#include <hbnsc.h>
#include <hbnsciconprovider.h>
#include <hbnsclicensemodel.h>

int main(int argc, char *argv[])
{
    std::unique_ptr<QGuiApplication> app(SailfishApp::application(argc, argv));

    std::unique_ptr<QQuickView> view(SailfishApp::createView());

    auto hbnscIconProvider = Hbnsc::HbnscIconProvider::createProvider(view->engine());

    view->setSource(SailfishApp::pathToMainQml());

    view->showFullScreen();

    return app->exec();
}
