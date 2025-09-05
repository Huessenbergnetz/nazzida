/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    allowedOrientations: defaultAllowedOrientations

    Column {
        anchors {
            left: parent.left
            leftMargin: Theme.horizontalPageMargin
            right: parent.right
            rightMargin: Theme.horizontalPageMargin
            verticalCenter: parent.verticalCenter
        }
        spacing: Theme.paddingSmall

        Label {
            width: parent.width
            //: label on a page that show errors during startup
            //% "Startup error"
            text: qsTrId("naz-label-startup-error")
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            width: parent.width
            font.pixelSize: Theme.fontSizeSmall
            horizontalAlignment: Text.AlignHCenter
            color: Theme.secondaryColor
            text: startupError
        }
    }
}
