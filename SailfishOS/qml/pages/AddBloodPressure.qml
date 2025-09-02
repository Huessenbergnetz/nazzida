/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0
import "../parts"

Dialog {
    id: addBloodPressureDialog
    allowedOrientations: defaultAllowedOrientations
    property Person person: null
    property BloodPressureListFilterModel bloodPressureModel: null
    property date moment: new Date()

    SilicaFlickable {
        id: addBloodPressureFlick
        anchors.fill: parent
        contentHeight: addBloodPressureCol.height

        VerticalScrollDecorator {
            flickable: addBloodPressureFlick
            page: addBloodPressureDialog
        }

        Column {
            id: addBloodPressureCol
            width: parent.width
            spacing: Theme.paddingLarge

            DialogHeader {
                flickable: addBloodPressureFlick
                // defined at BloodPressure.qml
                title: qsTrId("naz-add-blood-pressure")
                // defined at AddPerson.qml
                acceptText: qsTrId("naz-dialog-accept-add")
            }

            DateTimeField {
                id: momentField
                dateTime: addBloodPressureDialog.moment
                onDateTimeChanged: addBloodPressureDialog.moment = dateTime
            }

            Row {
                width: parent.width
                spacing: Theme.paddingSmall

                TextField {
                    id: systolicField
                    width: parent.width/2 - Theme.paddingSmall/2
                    //: text field label for entering systolic blood pressure
                    //% "Systolic mmHg"
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
                    width: parent.width/2 - Theme.paddingSmall/2
                    //: text field label for entering diastolic blood pressure
                    //% "Diastolic mmHg"
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
                width: parent.width
                //: text field label for entering blood pulse
                //% "Pulse in bpm"
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
                width: parent.width
                // defined in AddLiquid.qml
                label: qsTrId("naz-textarea-note-label"); placeholderText: label
                EnterKey.enabled: addBloodPressureDialog.canAccept
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: addBloodPressureDialog.accept()
            }
        }
    }

    canAccept: systolicField.text.length && systolicField.acceptableInput && diastolicField.text.length && diastolicField.acceptableInput

    onAccepted: bloodPressureModel.add(addBloodPressureDialog.moment, parseInt(systolicField.text), parseInt(diastolicField.text), parseInt(pulseField.text), noteArea.text)
}
