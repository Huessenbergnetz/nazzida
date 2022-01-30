/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0

Page {
    id: liquidsPage
    allowedOrientations: defaultAllowedOrientations
    property Person person: null

    SilicaListView {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                //: Dialog title and pull down menu entry, means liquids
                //% "Add In‐ or Output"
                text: qsTrId("naz-add-input-output")
            }
        }

        VerticalScrollDecorator {
            flickable: parent
            page: liquidsPage
        }

        header: PageHeader {
            title: qsTrId("naz-fluid-balance-protocol")
            description: qsTrId("naz-full-name").arg(person.firstName).arg(person.lastName)
        }
    }
}
