/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0

Page {
    id: settingsPage
    allowedOrientations: Orientation.All

    SilicaFlickable {
        id: settingsFlick
        anchors.fill: parent
        contentHeight: settingsCol.height

        VerticalScrollDecorator {
            flickable: settingsFlick
            page: settingsPage
        }

        Column {
            id: settingsCol
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }

            PageHeader {
                title: qsTrId("naz-title-settings")
                page: settingsPage
            }

            SectionHeader {
                //: settings section header
                //% "Appearance"
                text: qsTrId("naz-settings-section-appearance")
            }


            ComboBox {
                id: languagePicker
                //: ComboBox label on the settings page
                //% "Language"
                label: qsTrId("naz-settings-language-label")
                //: Description for a combo box on the settings page to select the application language
                //% "To change the language, you have to restart the application."
                description: qsTrId("naz-settings-language-desc")
                menu: ContextMenu {
                    Repeater {
                        model: LanguagesModel { id: langModel }
                        MenuItem { text: model.name; readonly property string value: model.code }
                    }
                }
                onCurrentIndexChanged: if (currentItem) { config.language = currentItem.value }
                currentIndex: langModel.findIndex(config.language)
            }
        }
    }
}
