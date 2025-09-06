/*
 * SPDX-FileCopyrightText: (C) 2022-2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2

/*
  Types:
  0 - New
  1 - Improved/Enhanced
  2 - Fixed
  3 - Note
*/

ListModel {
    ListElement {
        version: "1.1.0"
        date: 1757142397000
        entries: [
            ListElement { type: 0; issue: "8"; description: "log weight" },
            ListElement { type: 0; issue: "9"; description: "log blood pressure" },
            ListElement { type: 1; issue: "5"; description: "store datetimes as UTC in database" }
        ]
    }

    ListElement {
        version: "1.0.2"
        date: 1646680364000
        entries: [
            ListElement { type: 0; issue: ""; description: "Swedish translation by Åke Engelbrektson" },
            ListElement { type: 2; issue: "10"; description: "out of bounds crash when editing liquids time" },
            ListElement { type: 2; issue: "12"; description: "segmentation fault on shutdown" }
        ]
    }

    ListElement {
        version: "1.0.1"
        date: 1645439874000
        entries: [
            ListElement { type: 1; issue: "7"; description: "reorder input fields when adding/editing a person in a more logical way" },
            ListElement { type: 1; issue: "6"; description: "do not use current time as day start time for new person" },
            ListElement { type: 1; issue: "2"; description: "add view placeholder for empty views" },
            ListElement { type: 2; issue: "4"; description: "adding liquid on daily details page does not update the model" },
            ListElement { type: 2; issue: "3"; description: "fix missing scrollbar when editing person" },
            ListElement { type: 2; issue: "1"; description: "add English to the list of supported languages" }
        ]
    }

    ListElement {
        version: "1.0.0"
        date: 1645366646000
        entries: [
                ListElement { type: 3; issue: ""; description: "initial released version" }
        ]
    }
}
