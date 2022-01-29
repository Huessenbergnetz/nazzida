/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0

Page {
    id: sectionsPage
    allowedOrientations: Orientation.All
    property Person person: null

    SilicaFlickable {
        id: sectionsFlick
        anchors.fill: parent
        contentHeight: sectionsCol.height

        VerticalScrollDecorator {
            flickable: sectionsFlick
            page: sectionsPage
        }

        Column {
            id: sectionsCol
            anchors {
                left: parent.left
                right: parent.right
            }
            spacing: Theme.paddingSmall

            PageHeader {
                page: sectionsPage
                title: qsTrId("naz-full-name").arg(person.firstName).arg(person.lastName)
            }

            BackgroundItem {
                width: parent.width
                contentHeight: Theme.itemSizeMedium

                Icon {
                    id: drinkingIcon
                    anchors {
                        left: parent.left
                        leftMargin: Theme.horizontalPageMargin
                    }
                    source: "image://theme/icon-l-transfer"
                    width: Theme.iconSizeLarge
                    height: Theme.iconSizeLarge
                    highlighted: parent.highlighted
                }

                Label {
                    anchors {
                        left: drinkingIcon.right
                        leftMargin: Theme.paddingLarge
                        right: parent.ri
                        rightMargin: Theme.horizontalPageMargin
                        verticalCenter: drinkingIcon.verticalCenter
                    }
                    //: label on a list of different entries
                    //% "Fluid Balance Protol"
                    text: qsTrId("naz-fluid-balance-protocol")
                }
            }
        }
    }
}
