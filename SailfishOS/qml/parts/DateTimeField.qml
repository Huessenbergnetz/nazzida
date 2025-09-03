/*
 * SPDX-FileCopyrightText: (C) 2023 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0

Row {
    id: dateTimeField

    property date dateTime: new Date()

    property int year: dateTime.getFullYear()
    property int month: dateTime.getMonth()
    property int day: dateTime.getDate()
    property int hours: dateTime.getHours()
    property int minutes: dateTime.getMinutes()

    width: parent.width
    spacing: Theme.paddingSmall

    ValueButton {
        id: dateField
        width: parent.width/2 - Theme.paddingSmall/2
        //: text field label
        //% "Date"
        label: qsTrId("naz-textfield-date")
        value: Qt.formatDate(dateTimeField.dateTime)

        onClicked: {
            var dialog = pageStack.push("Sailfish.Silica.DatePickerDialog", {date: dateTimeField.dateTime})

            dialog.accepted.connect(function() {
                dateField.value = dialog.dateText
                dateTimeField.year = dialog.year
                dateTimeField.month = dialog.month - 1
                dateTimeField.day = dialog.day
                dateTimeField.dateTime = new Date(dateTimeField.year, dateTimeField.month, dateTimeField.day, dateTimeField.hours, dateTimeField.minutes)
            })
        }
    }

    ValueButton {
        id: timeField
        width: parent.width/2 - Theme.paddingSmall/2
        //: text field label
        //% "Time"
        label: qsTrId("naz-textfield-time")
        value: Qt.formatTime(dateTime)

        onClicked: {
            var dialog = pageStack.push("Sailfish.Silica.TimePickerDialog", {hour: dateTimeField.hours, minute: dateTimeField.minutes})

            dialog.accepted.connect(function() {
                timeField.value = dialog.timeText
                dateTimeField.hours = dialog.hour
                dateTimeField.minutes = dialog.minute
                dateTimeField.dateTime = new Date(dateTimeField.year, dateTimeField.month, dateTimeField.day, dateTimeField.hours, dateTimeField.minutes)
            })
        }
    }
}
