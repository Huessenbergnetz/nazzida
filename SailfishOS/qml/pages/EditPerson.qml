/*
 * SPDX-FileCopyrightText: (C) 2022-2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0

Dialog {
    id: editPersonDialog
    allowedOrientations: defaultAllowedOrientations
    property int modelIndex
    property PeopleListFilterModel peopleModel: null
    property Person person: null
    property date birthday: person.birthday
    property date dayStarts: person.dayStarts

    SilicaFlickable {
        id: editPersonFlick
        anchors.fill: parent
        contentHeight: editPersonCol.height

        VerticalScrollDecorator {
            flickable: parent
            page: editPersonDialog
        }

        Column {
            id: editPersonCol
            width: parent.width

            DialogHeader {
                flickable: editPersonFlick
                //: dialog header title, %1 will be replaced by the first name, %2 by the last name
                //% "Edit %1 %2"
                title: qsTrId("naz-title-edit-person").arg(person.firstName).arg(person.lastName)
                // defined in EditWeight.qml
                acceptText: qsTrId("naz-dialog-accept-edit")
            }

            TextField {
                id: firstNameField
                width: parent.width
                text: person.firstName
                label: qsTrId("naz-textfield-firstname"); placeholderText: label
                EnterKey.enabled: text.length
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: lastNameField.focus = true
            }

            TextField {
                id: lastNameField
                width: parent.width
                text: person.lastName
                label: qsTrId("naz-textfield-lastname"); placeholderText: label
                EnterKey.enabled: text.length
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: sizeField.focus = true
            }

            TextField {
                id: sizeField
                width: parent.width
                text: person.size
                label: qsTrId("naz-textfield-size"); placeholderText: label
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 0 }
                EnterKey.iconSource: "image://theme/icon-m-enter-close"
                EnterKey.onClicked: sizeField.focus = false
            }

            TextField {
                id: birthdayField
                width: parent.width
                readOnly: true
                text: Qt.formatDate(birthday)
                label: qsTrId("naz-textfield-birthday"); placeholderText: label
                onClicked: {
                    var dialog = pageStack.push(birthdayPickerComponent, {date: birthday})

                    dialog.accepted.connect(function() {
                        birthdayField.text = dialog.dateText
                        birthday = dialog.date
                    })
                }

                Component {
                    id: birthdayPickerComponent
                    DatePickerDialog {}
                }
            }

            ComboBox {
                id: sexBox
                label: qsTrId("naz-combo-label-sex")
                menu: ContextMenu {
                    id: sexBoxEntries
                    MenuItem {
                        text: qsTrId("naz-combo-sex-item-male")
                        readonly property string value: "m"
                    }
                    MenuItem {
                        text: qsTrId("naz-combo-sex-item-female")
                        readonly property string value: "f"
                    }
                    MenuItem {
                        text: qsTrId("naz-combo-sex-item-other")
                        readonly property string value: "x"
                    }
                }

                Component.onCompleted: {
                    var length = sexBoxEntries.children.length
                    for (var i = 0; i < length; ++i) {
                        if (sexBoxEntries.children[i].value === person.sex) {
                            sexBox.currentIndex = i
                            break
                        }
                    }
                }
            }

            TextField {
                id: transpireField
                width: parent.width
                text: person.transpire
                label: qsTrId("naz-textfield-transpire"); placeholderText: label
                description: qsTrId("naz-textfield-transpire-desc")
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 0 }
                EnterKey.iconSource: "image://theme/icon-m-enter-close"
                EnterKey.onClicked: transpireField.focus = false
            }

            TextField {
                id: dayStartsField
                width: parent.width
                readOnly: true
                text: Qt.formatTime(dayStarts)
                label: qsTrId("naz-textfield-daystarts"); placeholderText: label
                description: qsTrId("naz-textfield-daystarts-desc")
                onClicked: {
                    var dialog = pageStack.push(dayStartsPickerComponent, {hour: dayStarts.getHours(), minute: dayStarts.getMinutes()})

                    dialog.accepted.connect(function() {
                        dayStartsField.text = dialog.timeText
                        dayStarts = new Date(1970, 1, 1, dialog.hour, dialog.minute, 0)
                    })
                }

                Component {
                    id: dayStartsPickerComponent
                    TimePickerDialog {}
                }
            }
        }
    }

    canAccept: firstNameField.text.length && lastNameField.text.length

    onAccepted: {
        peopleModel.edit(peopleModel.index(modelIndex,0), firstNameField.text, lastNameField.text, parseInt(sizeField.text), birthday, dayStarts, sexBox.currentItem.value, parseInt(transpireField.text))
    }
}
