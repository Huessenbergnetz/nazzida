/*
 * SPDX-FileCopyrightText: (C) 2022-2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0

Page {
    id: sectionsPage
    allowedOrientations: defaultAllowedOrientations
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
                description: (person.sex === "m" ? "♂︎" : person.sex === "f" ? "♀︎" : "⚧︎") + " " + qsTrId("naz-size-in-cm").arg(person.size) + " " + qsTrId("naz-age-in-years", person.age)
            }

            BackgroundItem {
                width: parent.width
                height: Theme.itemSizeMedium
                contentHeight: Theme.itemSizeMedium

                onClicked: pageStack.animatorPush(Qt.resolvedUrl("DailyLiquids.qml"), {person: sectionsPage.person})

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
                        right: parent.right
                        rightMargin: Theme.horizontalPageMargin
                        verticalCenter: drinkingIcon.verticalCenter
                    }
                    //: label on a list of different entries
                    //% "Fluid Balance Protocol"
                    text: qsTrId("naz-fluid-balance-protocol")
                }
            }

            BackgroundItem {
                width: parent.width
                height: Theme.itemSizeMedium
                contentHeight: Theme.itemSizeMedium

                onClicked: pageStack.animatorPush(Qt.resolvedUrl("Weights.qml"), {person: sectionsPage.person})

                Icon {
                    id: scaleIcon
                    anchors {
                        left: parent.left
                        leftMargin: Theme.horizontalPageMargin
                    }
                    source: "image://nazzida/icon-l-scale"
                    width: Theme.iconSizeLarge
                    height: Theme.iconSizeLarge
                    highlighted: parent.highlighted
                }

                Label {
                    anchors {
                        left: scaleIcon.right
                        leftMargin: Theme.paddingLarge
                        right: parent.right
                        rightMargin: Theme.horizontalPageMargin
                        verticalCenter: scaleIcon.verticalCenter
                    }
                    //: label on a list of different entries
                    //% "Weight Protocol"
                    text: qsTrId("naz-weight-protocol")
                }
            }
        }
    }
}
