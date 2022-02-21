/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0


Dialog {
    id: addPersonDialog
    allowedOrientations: defaultAllowedOrientations
    property PeopleListFilterModel peopleModel: null
    property date birthDate: new Date()
    property date dayStarts: new Date(1970, 0, 1, 7, 0, 0)

    SilicaFlickable {
        id: addPersonFlick
        anchors.fill: parent
        contentHeight: addPersonCol.height

        VerticalScrollDecorator {
            flickable: parent
            page: addPersonDialog
        }

        Column {
            id: addPersonCol
            width: parent.width

            DialogHeader {
                flickable: addPersonFlick
                title: qsTrId("naz-title-add-person")
                //: Accept text for add person dialog
                //% "Add"
                acceptText: qsTrId("naz-accept-add-person")
            }

            TextField {
                id: firstNameField
                width: parent.width
                //: text field label
                //% "First name"
                label: qsTrId("naz-textfield-firstname"); placeholderText: label
                EnterKey.enabled: text.length
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: lastNameField.focus = true
            }

            TextField {
                id: lastNameField
                width: parent.width
                //: text field label
                //% "Last name"
                label: qsTrId("naz-textfield-lastname"); placeholderText: label
                EnterKey.enabled: text.length
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: sizeField.focus = true
            }

            TextField {
                id: sizeField
                width: parent.width
                text: "175"
                //: text field label
                //% "Size in cm"
                label: qsTrId("naz-textfield-size"); placeholderText: label
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 0 }
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: transpireField.focus = true
            }

            TextField {
                id: transpireField
                width: parent.width
                text: "500"
                //: text field label
                //% "Transpire in ml"
                label: qsTrId("naz-textfield-transpire"); placeholderText: label
                //: text field description
                //% "Approximate daily transpiration. The body of an adult loses about 500 ml daily through perspiration. This is used when calculating the difference between fluid intake and fluid output."
                description: qsTrId("naz-textfield-transpire-desc")
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 0 }
                EnterKey.iconSource: "image://theme/icon-m-enter-close"
                EnterKey.onClicked: transpireField.focus = false
            }

            TextField {
                id: birthdayField
                width: parent.width
                readOnly: true
                text: Qt.formatDate(birthDate)
                //: text field label
                //% "Birthday"
                label: qsTrId("naz-textfield-birthday"); placeholderText: label
                onClicked: {
                    var dialog = pageStack.push(birthdayPickerComponent)

                    dialog.accepted.connect(function() {
                        birthdayField.text = dialog.dateText
                        birthDate = dialog.date
                    })
                }

                Component {
                    id: birthdayPickerComponent
                    DatePickerDialog {}
                }
            }

            TextField {
                id: dayStartsField
                width: parent.width
                readOnly: true
                text: Qt.formatTime(dayStarts)
                //: text field label
                //% "Day start time"
                label: qsTrId("naz-textfield-daystarts"); placeholderText: label
                //: text field description
                //% "The day start time is used to determine when a new day starts. Liquid intake or output that occurs before this time will be assigned to the previous day. The first toilet after getting up is usually still assigned to the balance of the previous day. So you should choose a time when you nominally get up."
                description: qsTrId("naz-textfield-daystarts-desc")
                onClicked: {
                    var dialog = pageStack.push(dayStartsPickerComponent, {hour: dayStarts.getHours(), minute: dayStarts.getMinutes()})

                    dialog.accepted.connect(function() {
                        dayStartsField.text = dialog.timeText
                        dayStarts = dialog.time
                    })
                }

                Component {
                    id: dayStartsPickerComponent
                    TimePickerDialog {}
                }
            }

            ComboBox {
                id: sexBox
                //: ComboBox label
                //% "Sex"
                label: qsTrId("naz-combo-label-sex")
                menu: ContextMenu {
                    MenuItem {
                        //: ComboBox entry
                        //% "Male"
                        text: qsTrId("naz-combo-sex-item-male")
                        readonly property string value: "m"
                    }
                    MenuItem {
                        //: ComboBox entry
                        //% "Female"
                        text: qsTrId("naz-combo-sex-item-female")
                        readonly property string value: "f"
                    }
                    MenuItem {
                        //: ComboBox entry
                        //% "Other"
                        text: qsTrId("naz-combo-sex-item-other")
                        readonly property string value: "x"
                    }
                }
            }
        }
    }

    onAccepted: {
        peopleModel.add(firstNameField.text, lastNameField.text, parseInt(sizeField.text), birthDate, dayStarts, sexBox.currentItem.value, parseInt(transpireField.text))
    }

    canAccept: firstNameField.text.length && lastNameField.text.length
}
