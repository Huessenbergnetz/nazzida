/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0

Dialog {
    id: editLiquidDialog
    allowedOrientations: defaultAllowedOrientations

    property Person person: null
    property LiquidListFilterModel liquidsModel: null
    property DailyLiquidListFilterModel dailyLiquidsModel: null
    property int modelIndex
    property date moment
    property int inOrOut
    property int amount
    property string name
    property string note

    property date oldMoment
    property int oldInOrOut
    property int oldAmount

    Component.onCompleted: {
        oldMoment = moment
        oldInOrOut = inOrOut
        oldAmount = amount
    }

    SilicaFlickable {
        id: editLiquidFlick
        anchors.fill: parent
        contentHeight: editLiquidCol.height

        VerticalScrollDecorator {
            flickable: parent
            page: editLiquidDialog
        }

        Column {
            id: editLiquidCol
            width: parent.width
            spacing: Theme.paddingLarge

            DialogHeader {
                flickable: editLiquidFlick
                //: dialog title
                //% "Edit In- or Output"
                title: qsTrId("naz-edit-input-output")
                //: Accept text on liquid edit dialog
                //% "Apply"
                acceptText: qsTrId("naz-accept-edit-liquid")
            }

            Item {
                width: parent.width
                height: Math.max(inOutIcon.height, typePicker.height)

                Icon {
                    id: inOutIcon
                    anchors {
                        left: parent.left
                        leftMargin: Theme.horizontalPageMargin
                    }
                    source: typePicker.currentItem.value === Liquid.In ? "image://nazzida/icon-l-cup" : typePicker.currentItem.value === Liquid.InIV ? "image://nazzida/icon-l-syringe" : "image://nazzida/icon-l-peeing"
                    width: Theme.iconSizeLarge
                    height: Theme.iconSizeLarge
                }

                ComboBox {
                    id: typePicker
                    anchors {
                        left: inOutIcon.right
                        leftMargin: Theme.paddingLarge
                        right: parent.right
                        rightMargin: Theme.horizontalPageMargin
                    }
                    //: label for a combobox on th add liquid dialog
                    //% "In‐ or Output"
                    label: qsTrId("naz-combo-label-inouttype")
                    menu: ContextMenu {
                        id: typePickerEntries
                        MenuItem {
                            //: ComboBox item
                            //% "Drinks/Soups"
                            text: qsTrId("naz-combo-inouttype-item-in")
                            readonly property int value: Liquid.In
                        }
                        MenuItem {
                            //: ComboBox item
                            //% "Intravenous"
                            text: qsTrId("naz-combo-inouttype-item-iniv")
                            readonly property int value: Liquid.InIV
                        }
                        MenuItem {
                            //: ComboBox item, excretion of body fluids
                            //% "Excretion"
                            text: qsTrId("naz-combo-inouttype-item-out")
                            readonly property int value: Liquid.Out
                        }
                    }

                    Component.onCompleted: {
                        var l = typePickerEntries.children.length
                        for (var i = 0; i < l; ++i) {
                            if (typePickerEntries.children[i].value === editLiquidDialog.inOrOut) {
                                typePicker.currentIndex = i
                                break
                            }
                        }
                    }
                }
            }

            Item {
                width: parent.width
                height: Math.max(dateField.height, timeField.height)

                TextField {
                    id: dateField
                    property int year: editLiquidDialog.moment.getFullYear()
                    property int month: editLiquidDialog.moment.getMonth()
                    property int day: editLiquidDialog.moment.getDate()
                    anchors {
                        left: parent.left
                    }
                    width: parent.width/2 - Theme.horizontalPageMargin - Theme.paddingLarge/2
                    text: Qt.formatDate(editLiquidDialog.moment)
                    readOnly: true
                    label: qsTrId("naz-textfield-inoutput-date"); placeholderText: label

                    onClicked: {
                        var dialog = pageStack.push(datePickerComp, {date: editLiquidDialog.moment})

                        dialog.accepted.connect(function() {
                            dateField.text = dialog.dateText
                            dateField.year = dialog.year
                            dateField.month = dialog.month - 1
                            dateField.day = dialog.day
                            editLiquidDialog.moment = new Date(dateField.year, dateField.month, dateField.day, timeField.hour, timeField.minute)
                        })
                    }

                    Component {
                        id: datePickerComp
                        DatePickerDialog {}
                    }
                }

                TextField {
                    id: timeField
                    property int hour: editLiquidDialog.moment.getHours()
                    property int minute: editLiquidDialog.moment.getMinutes()
                    anchors {
                        left: dateField.right
                        leftMargin: Theme.paddingLarge
                        right: parent.right
                        rightMargin: Theme.horizontalPageMargin
                    }
                    text: Qt.formatTime(editLiquidDialog.moment)
                    readOnly: true
                    label: qsTrId("naz-textfield-inoutput-time"); placeholderText: label

                    onClicked: {
                        var dialog = pageStack.push(timePickerComp, {hour: editLiquidDialog.moment.getHours(), minute: editLiquidDialog.moment.getMinutes()})

                        dialog.accepted.connect(function() {
                            timeField.text = dialog.timeText
                            timeField.hour = dialog.hour
                            timeField.minute = dialog.minute
                            editLiquidDialog.moment = new Date(dateField.year, dateField.month, dateField.day, timeField.hour, timeField.minute)
                        })
                    }

                    Component {
                        id: timePickerComp
                        TimePickerDialog {}
                    }
                }
            }

            TextField {
                id: amountField
                width: parent.width
                label: qsTrId("naz-textfield-liquid-amount-label"); placeholderText: label
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 0 }
                text: editLiquidDialog.amount
                EnterKey.enabled: text.length
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: nameField.focus = true
            }

            TextField {
                id: nameField
                width: parent.width
                label: qsTrId("naz-textfield-liquid-name-label"); placeholderText: label
                text: editLiquidDialog.name
                EnterKey.enabled: text.length
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: noteArea.focus = true
            }

            TextArea {
                id: noteArea
                width: parent.width
                label: qsTrId("naz-textarea-note-label"); placeholderText: label
                text: editLiquidDialog.note
                EnterKey.enabled: editLiquidDialog.canAccept
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: editLiquidDialog.accept()
            }
        }
    }

    canAccept: nameField.text.length && amountField.text.length && parseInt(amountField.text) > 0

    onAccepted: {
        if (liquidsModel.edit(liquidsModel.index(editLiquidDialog.modelIndex,0), editLiquidDialog.moment, typePicker.currentItem.value, parseInt(amountField.text), nameField.text, noteArea.text)) {
            dailyLiquidsModel.edit(editLiquidDialog.oldMoment, editLiquidDialog.moment, editLiquidDialog.oldInOrOut, typePicker.currentItem.value, editLiquidDialog.oldAmount, parseInt(amountField.text))
        }
    }
}
