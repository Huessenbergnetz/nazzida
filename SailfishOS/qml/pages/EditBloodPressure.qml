/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0
import "../parts"

Dialog {
    id: editBloodPressureDialog
    allowedOrientations: defaultAllowedOrientations

    property Person person: null
    property BloodPressureListFilterModel bloodPressureModel: null

    property int modelIndex
    property date moment
    property int systolic
    property int diastolic
    property int pulse
    property string note

    SilicaFlickable {
        id: editBloodPressureFlick
        anchors.fill: parent
        contentHeight: editBloodPressureCol.height

        VerticalScrollDecorator {
            flickable: parent
            page: editBloodPressureDialog
        }

        Column {
            id: editBloodPressureCol
            width: parent.width
            spacing: Theme.paddingLarge

            DialogHeader {
                flickable: editBloodPressureFlick
                //: dialog title
                //% "Edit Blood Pressure"
                title: qsTrId("naz-edit-blood-pressure")
                // defined in EditWeight.qml
                acceptText: qsTrId("naz-dialog-accept-edit")
            }

            DateTimeField {
                id: momentField
                dateTime: editBloodPressureDialog.moment
                onDateTimeChanged: editBloodPressureDialog.moment = dateTime
            }

            Row {
                width: parent.width
                spacing: Theme.paddingSmall

                TextField {
                    id: systolicField
                    text: editBloodPressureDialog.systolic
                    width: parent.width/2 - Theme.paddingSmall/2
                    // defined in AddBloodPressure.qml
                    label: qsTrId("naz-textfield-blood-pressure-systolic"); placeholderText: label
                    validator: IntValidator {
                        bottom: 0
                        top: 500
                    }
                    inputMethodHints: Qt.ImhDigitsOnly
                    EnterKey.enabled: text.length > 0 && acceptableInput
                    EnterKey.iconSource: "image://theme/icon-m-enter-next"
                    EnterKey.onClicked: diastolicField.focus = true
                }

                TextField {
                    id: diastolicField
                    text: editBloodPressureDialog.diastolic
                    width: parent.width/2 - Theme.paddingSmall/2
                    // defined in AddBloodPressure.qml
                    label: qsTrId("naz-textfield-blood-pressure-diastolic"); placeholderText: label
                    validator: IntValidator {
                        bottom: 0
                        top: 500
                    }
                    inputMethodHints: Qt.ImhDigitsOnly
                    EnterKey.enabled: text.length > 0 && acceptableInput
                    EnterKey.iconSource: "image://theme/icon-m-enter-next"
                    EnterKey.onClicked: pulseField.focus = true
                }
            }

            TextField {
                id: pulseField
                text: editBloodPressureDialog.pulse
                width: parent.width
                // defined in AddBloddPressure.qml
                label: qsTrId("naz-textfield-blood-pulse"); placeholderText: label
                validator: IntValidator {
                    bottom: 0
                    top: 500
                }
                inputMethodHints: Qt.ImhDigitsOnly
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: noteArea.focus = true
            }

            TextArea {
                id: noteArea
                text: editBloodPressureDialog.note
                width: parent.width
                // defined in AddLiquid.qml
                label: qsTrId("naz-textarea-note-label"); placeholderText: label
                EnterKey.enabled: editBloodPressureDialog.canAccept
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: editBloodPressureDialog.accept()
            }
        }
    }

    canAccept: systolicField.text.length && systolicField.acceptableInput && diastolicField.text.length && diastolicField.acceptableInput

    onAccepted: bloodPressureModel.edit(bloodPressureModel.index(editBloodPressureDialog.modelIndex, 0), editBloodPressureDialog.moment, parseInt(systolicField.text), parseInt(diastolicField.text), parseInt(pulseField.text), noteArea.text)
}
