/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0


Dialog {
    id: addLiquidDialog
    allowedOrientations: defaultAllowedOrientations
    property Person person: null
    property LiquidListFilterModel liquidsModel: null
    property date moment: new Date()

    SilicaFlickable {
        id: addLiquidFlick
        anchors.fill: parent
        contentHeight: addLiquidCol.height

        VerticalScrollDecorator {
            flickable: parent
            page: addLiquidDialog
        }

        Column {
            id: addLiquidCol
            width: parent.width
            spacing: Theme.paddingLarge

            DialogHeader {
                flickable: addLiquidFlick
                title: qsTrId("naz-add-input-output")
                //: Accept text for add in- or output dialog
                //% "Add"
                acceptText: qsTrId("naz-accept-add-inoutput")
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
                }
            }

            Item {
                width: parent.width
                height: Math.max(dateField.height, timeField.height)

                TextField {
                    id: dateField
                    property int year: moment.getFullYear()
                    property int month: moment.getMonth()
                    property int day: moment.getDate()
                    anchors {
                        left: parent.left
                    }
                    width: parent.width/2 - Theme.horizontalPageMargin - Theme.paddingLarge/2
                    text: Qt.formatDate(moment)
                    readOnly: true
                    //: text field label
                    //% "Date"
                    label: qsTrId("naz-textfield-inoutput-date"); placeholderText: label

                    onClicked: {
                        var dialog = pageStack.push(datePickerComp, {date: moment})

                        dialog.accepted.connect(function() {
                            dateField.text = dialog.dateText
                            dateField.year = dialog.year
                            dateField.month = dialog.month - 1
                            dateField.day = dialog.day
                            moment = new Date(dateField.year, dateField.month, dateField.day, timeField.hour, timeField.minute)
                        })
                    }

                    Component {
                        id: datePickerComp
                        DatePickerDialog {}
                    }
                }

                TextField {
                    id: timeField
                    property int hour: moment.getHours()
                    property int minute: moment.getMinutes()
                    anchors {
                        left: dateField.right
                        leftMargin: Theme.paddingLarge
                        right: parent.right
                        rightMargin: Theme.horizontalPageMargin
                    }
                    text: Qt.formatTime(moment)
                    readOnly: true
                    //: text field label
                    //% "Time"
                    label: qsTrId("naz-textfield-inoutput-time"); placeholderText: label

                    onClicked: {
                        var dialog = pageStack.push(timePickerComp, {hour: moment.getHours(), minute: moment.getMinutes()})

                        dialog.accepted.connect(function() {
                            timeField.text = dialog.timeText
                            timeField.hour = dialog.hour
                            timeField.minute = dialog.minute
                            moment = new Date(dateField.year, dateField.month, dateField.day, timeField.hour, timeField.minute)
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
                //: text field label
                //% "Amount in ml"
                label: qsTrId("naz-textfield-liquid-amount-label"); placeholderText: label
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 0 }
                EnterKey.enabled: text.length
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: nameField.focus = true
            }

            TextField {
                id: nameField
                width: parent.width
                //: text field label
                //% "Name"
                label: qsTrId("naz-textfield-liquid-name-label"); placeholderText: label
                EnterKey.enabled: text.length
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: noteArea.focus = true
            }

            TextArea {
                id: noteArea
                width: parent.width
                //: text area label
                //% "Note (optional)"
                label: qsTrId("naz-textarea-liquid-note-label"); placeholderText: label
                EnterKey.enabled: addLiquidDialog.canAccept
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: addLiquidDialog.accept()
            }
        }
    }

    canAccept: nameField.text.length && amountField.text.length && parseInt(amountField.text) > 0

    onAccepted: liquidsModel.add(moment, typePicker.currentItem.value, parseInt(amountField.text), nameField.text, noteArea.text)
}
