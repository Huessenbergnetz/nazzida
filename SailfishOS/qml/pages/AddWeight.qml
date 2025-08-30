/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0
import "../parts"

Dialog {
    id: addWeightDialog
    allowedOrientations: defaultAllowedOrientations
    property Person person: null
    property WeightListFilterModel weightsModel: null
    property date moment: new Date()

    SilicaFlickable {
        id: addWeightFlick
        anchors.fill: parent
        contentHeight: addWeightCol.height

        VerticalScrollDecorator {
            flickable: addWeightFlick
            page: addWeightDialog
        }

        Column {
            id: addWeightCol
            width: parent.width
            spacing: Theme.paddingLarge

            DialogHeader {
                flickable: addWeightFlick
                title: qsTrId("naz-add-weight")
            }

            DateTimeField {
                id: momentField
                dateTime: addWeightDialog.moment
                onDateTimeChanged: addWeightDialog.moment = dateTime
            }

            TextField {
                id: weightField
                width: parent.width
                //: text field label, %1 will be replaced by the unit, like kg
                //% "Weight in %1"
                label: qsTrId("naz-textfield-weight-label").arg("kg"); placeholderText: label
                inputMethodHints: Qt.ImhDigitsOnly
                validator: DoubleValidator {bottom: 0.00; top: 600.00; notation: DoubleValidator.StandardNotation; decimals: 1}
                EnterKey.enabled: text.length
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: noteArea.focus = true
            }

            TextArea {
                id: noteArea
                width: parent.width
                //: text area label
                //% "Note (optional)"
                label: qsTrId("naz-textarea-note-label"); placeholderText: label
                EnterKey.enabled: addWeightDialog.canAccept
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: addWeightDialog.accept()
            }
        }
    }

    canAccept: weightField.text.length && parseFloat(Number.fromLocaleString(Qt.locale(), weightField.text)) > 0.0

    onAccepted: weightsModel.add(addWeightDialog.moment, Math.round(parseFloat(Number.fromLocaleString(Qt.locale(), weightField.text)) * 1000), noteArea.text)
}
