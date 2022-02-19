/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import "../models"

Page {
    id: privacyPolicyPage

    SilicaFlickable {
        id: privacyPolicyFlick

        anchors.fill: parent
        contentHeight: privacyPolicyCol.height

        VerticalScrollDecorator {
            flickable: privacyPolicyFlick
            page: privacyPolicyPage
        }

        Column {
            id: privacyPolicyCol
            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                page: privacyPolicyPage
                //: Label and page header
                //% "Privacy policy"
                title: qsTrId("naz-privazy-policy")
            }

            Repeater {
                model: PrivacyPolicyModel {}

                delegate: Text {
                    anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
                    color: model.header ? Theme.highlightColor : Theme.primaryColor
                    font.pixelSize: model.header ? Theme.fontSizeMedium : Theme.fontSizeSmall
                    textFormat: model.format ? model.format : Text.PlainText
                    onLinkActivated: Qt.openUrlExternally(link)
                    text: model.text
                    wrapMode: Text.WordWrap
                    linkColor: Theme.highlightColor
                }
            }

            Item {
                width: parent.width
                height: Theme.paddingLarge
            }
        }
    }
}
