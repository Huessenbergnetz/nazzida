/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2

ListModel {
    id: privacyPolicyModel

    ListElement {
        header: true
        text: ""
    }

    ListElement {
        text: ""
    }

    ListElement {
        header: true
        text: ""
    }

    ListElement {
        text: ""
    }

    ListElement {
        header: true
        text: ""
    }

    ListElement {
        text: ""
    }

    ListElement {
        header: true
        text: ""
    }

    ListElement {
        text: ""
    }

    ListElement {
        header: true
        text: ""
    }

    ListElement {
        format: Text.StyledText
        text: "<a href='https://www.huessenbergnetz.de/?pk_campaign=Sailfish-App-Nazzida&pk_kwd=PrivacyPolicy'>https://www.huessenbergnetz.de</a>"
    }

    Component.onCompleted: {
        //% "What information does Nazzida collect?"
        privacyPolicyModel.get(0).text = qsTrId("naz-privacy-0")
        //% "Nazzida helps you to manage a personal fluid balance protocol if you need to log your daily liquid input and output. To do this, it stores your input into a local SQLite database on your device. It saves only data that you enter yourself in the application."
        privacyPolicyModel.get(1).text = qsTrId("naz-privacy-1")

        //% "What does Nazzida use your data for?"
        privacyPolicyModel.get(2).text = qsTrId("naz-privacy-2")
        //% "Nazzida uses your stored data to show you your daily liquid balance data as well as every data entry you provided to the application. The data is only stored on your local device and is not shared to other parties."
        privacyPolicyModel.get(3).text = qsTrId("naz-privacy-3")

        //% "Does Nazzida disclose any information to outside parties?"
        privacyPolicyModel.get(4).text = qsTrId("naz-privacy-4")
        //% "No, Nazzida only stores the data local on your device. Nazzida does not send this data to the author of Nazzida or any other third parties."
        privacyPolicyModel.get(5).text = qsTrId("naz-privacy-5")

        //% "Your consent"
        privacyPolicyModel.get(6).text = qsTrId("nazh-privacy-6")
        //% "By using Nazzida, you consent to this privacy policy."
        privacyPolicyModel.get(7).text = qsTrId("nazh-privacy-7")

        //% "Contact"
        privacyPolicyModel.get(8).text = qsTrId("nazh-privacy-8")
    }
}
