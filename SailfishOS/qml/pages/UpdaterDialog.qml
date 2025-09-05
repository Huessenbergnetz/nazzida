/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0

Dialog {
    id: updaterDialog
    acceptDestination: Qt.resolvedUrl("FirstPage.qml")
    acceptDestinationAction: PageStackAction.Replace
    allowedOrientations: defaultAllowedOrientations
    backNavigation: false
    canAccept: updater.hasFinished && !updater.hasError

    Updater {
        id: updater
    }

    SilicaFlickable {
        id: updaterFlick
        anchors.fill: parent
        contentHeight: updaterCol.height

        VerticalScrollDecorator {
            flickable: parent
            page: updaterDialog
        }

        Column {
            id: updaterCol
            width: parent.width
            spacing: Theme.paddingLarge

            DialogHeader {
                flickable: updaterFlick
                //: dialog header title
                //% "Update Nazzida"
                title: qsTrId("naz-title-updater")
                //: Accept text for dialog
                //% "Next"
                acceptText: qsTrId("naz-dialog-accept-next")
            }

            Text {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                //% "Your database needs to be upgraded."
                text: qsTrId("naz-updater-description")
            }

            Label {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                horizontalAlignment: Text.AlignHCenter
                //: label when db update was successful
                //% "Update successful"
                text: qsTrId("naz-updater-successful")
                visible: updater.hasFinished && !updater.hasError
            }

            Button {
                //: button text
                //% "Start update"
                text: qsTrId("naz-updater-start-update")
                anchors.horizontalCenter: parent.horizontalCenter
                visible: !updater.inOperation && !updater.hasFinished && !updater.hasError
                onClicked: updater.startUpdate()
            }

            BusyLabel {
                text: updater.currentAction
                running: updater.inOperation
            }

            Label {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                horizontalAlignment: Text.AlignHCenter
                //: label when error occured during db update
                //% "Update error"
                text: qsTrId("naz-updater-error");
                visible: updater.hasError
            }

            Text {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                horizontalAlignment: Text.AlignHCenter
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: updater.error
                visible: updater.hasError
                wrapMode: Text.WordWrap
            }
        }
    }
}
