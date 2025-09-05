/*
 * SPDX-FileCopyrightText: (C) 2022-2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0

ApplicationWindow {
    initialPage: startupError
                 ? Qt.resolvedUrl("pages/StartupError.qml")
                 : isUpdateRequired
                   ? Qt.resolvedUrl("pages/UpdaterDialog.qml")
                   : Qt.resolvedUrl("pages/FirstPage.qml")
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations
}
