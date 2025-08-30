/*
 * SPDX-FileCopyrightText: (C) 2023 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0

Item {
    id: dateTimeField

    property date dateTime: new Date()

    property int year: dateTime.getFullYear()
    property int month: dateTime.getMonth()
    property int day: dateTime.getDate()
    property int hours: dateTime.getHours()
    property int minutes: dateTime.getMinutes()

    width: parent.width
    height: Math.max(dateField.height, timeField.height)

    TextField {
        id: dateField
        anchors.left: parent.left
        width: parent.width/2 - Theme.horizontalPageMargin - Theme.paddingLarge/2
        text: Qt.formatDate(dateTime)
        readOnly: true
        //: text field label
        //% "Date"
        label: qsTrId("naz-textfield-date"); placeholderText: label

        onClicked: {
            var dialog = pageStack.push(datePickerComp, {date: dateTimeField.dateTime})

            dialog.accepted.connect(function() {
                dateField.text = dialog.dateText
                dateTimeField.year = dialog.year
                dateTimeField.month = dialog.month - 1
                dateTimeField.day = dialog.day
                dateTimeField.dateTime = new Date(dateTimeField.year, dateTimeField.month, dateTimeField.day, dateTimeField.hours, dateTimeField.minutes)
            })
        }

        Component {
            id: datePickerComp
            DatePickerDialog {}
        }
    }

    TextField {
        id: timeField
        anchors {
            left: dateField.right
            leftMargin: Theme.paddingLarge
            right: parent.right
            rightMargin: Theme.horizontalPageMargin
        }
        text: Qt.formatTime(dateTime)
        readOnly: true
        //: text field label
        //% "Time"
        label: qsTrId("naz-textfield-time"); placeholderText: label

        onClicked: {
            var dialog = pageStack.push("Sailfish.Silica.TimePickerDialog", {hour: dateTimeField.hours, minute: dateTimeField.minutes})

            dialog.accepted.connect(function() {
                timeField.text = dialog.timeText
                dateTimeField.hours = dialog.hour
                dateTimeField.minutes = dialog.minute
                dateTimeField.dateTime = new Date(dateTimeField.year, dateTimeField.month, dateTimeField.day, dateTimeField.hours, dateTimeField.minutes)
            })
        }

//        Component {
//            id: timePickerComp
//            TimePickerDialog {}
//        }
    }
}
