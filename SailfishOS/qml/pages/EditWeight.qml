/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0
import "../parts"

Dialog {
     id: editWeightDialog
     allowedOrientations: defaultAllowedOrientations

     property Person person: null
     property WeightListFilterModel weightModel: null

     property int modelIndex
     property date moment
     property int weight
     property string note

     property date oldMoment
     property int oldWeight

     Component.onCompleted: {
         oldMoment = moment
         oldWeight = weight
     }

     SilicaFlickable {
         id: editWeightFlick
         anchors.fill: parent
         contentHeight: editWeightCol.height

         VerticalScrollDecorator {
             flickable: parent
             page: editWeightDialog
         }

         Column {
             id: editWeightCol
             width: parent.width
             spacing: Theme.paddingLarge

             DialogHeader {
                 flickable: editWeightFlick
                 //: dialog title
                 //% "Edit Weight"
                 title: qsTrId("naz-edit-weight")
                 //: Accept text on edit dialog headers
                 //% "Apply"
                 acceptText: qsTrId("naz-dialog-accept-edit")
             }

             DateTimeField {
                 id: momentField
                 dateTime: editWeightDialog.moment
                 onDateTimeChanged: editWeightDialog.moment = dateTime
             }

             TextField {
                 id: weightField
                 width: parent.width
                 label: qsTrId("naz-textfield-weight-label").arg("kg"); placeholderText: label
                 inputMethodHints: Qt.ImhDigitsOnly
                 validator: DoubleValidator {bottom: 0.00; top: 600.00; notation: DoubleValidator.StandardNotation; decimals: 1}
                 text: Number(editWeightDialog.weight / 1000.0).toLocaleString(Qt.locale(), 'f', 1)
                 EnterKey.enabled: text.length
                 EnterKey.iconSource: "image://theme/icon-m-enter-next"
                 EnterKey.onClicked: noteArea.focus = true
             }

             TextArea {
                 id: noteArea
                 width: parent.width
                 // defined in AddLiquid.qml
                 label: qsTrId("naz-textarea-note-label"); placeholderText: label
                 EnterKey.enabled: editWeightDialog.canAccept
                 EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                 EnterKey.onClicked: editWeightDialog.accept()
             }
         }
     }

     canAccept: weightField.text.length && parseFloat(Number.fromLocaleString(Qt.locale(), weightField.text)) > 0.0

     onAccepted: weightModel.edit(weightModel.index(editWeightDialog.modelIndex, 0), editWeightDialog.moment, Math.round(parseFloat(Number.fromLocaleString(Qt.locale(), weightField.text)) * 1000), noteArea.text)
}
