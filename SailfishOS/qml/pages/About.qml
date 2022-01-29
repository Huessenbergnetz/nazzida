/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import de.huessenbergnetz.hbnsc 1.0
import harbour.nazzida 1.0
import "../models"

AboutPage {
    allowedOrientations: defaultAllowedOrientations
    pageTitle: qsTrId("naz-title-about")
    appTitle: "Nazzida"
    //% "Nazzida is the old high german word for liquid. This applicaton helps you to manage a personal fluid balance protocol if you need to log your daily liquid input and output."
    appDescription: qsTrId("naz-about-app-desc")
    appHomepage: "https://github.com/Huessenbergnetz/nazzida"
    appCopyrightYearFrom: "2022"
    appCopyrightYearTo: "2022"
    appCopyrightHolder: "Matthias Fehring"
    appLicense: "GNU General Public License, Version 3"
    appLicenseFile: "GPLv3.qml"

    contactCompany: "Hüssenbergnetz"
    contactName: "Matthias Fehring"
    contactStreet: "Zum Südholz"
    contactHouseNo: "8"
    contactZIP: "34439"
    contactCity: "Willebadessen-Eissen"
    //% "Germany"
    contactCountry: qsTrId("naz-germany")
    contactEmail: Qt.atob("a29udGFrdEBodWVzc2VuYmVyZ25ldHouZGU=")
    contactWebsite: "www.huessenbergnetz.de"
    contactWebsiteLink: "https://www.huessenbergnetz.de/?pk_campaign=Sailfish-App-Nazzida&pk_kwd=AboutPage"

    bugUrl: "https://github.com/Huessenbergnetz/nazzida/issues"
    translateUrl: "https://www.transifex.com/huessenbergnetz/nazzida"
    sourceCodeUrl: "https://github.com/Huessenbergnetz/nazzida"

    licensesModel: LicensesModel {}

    contributorsModel: ContributorsModel {}

    changelogModel: ChangelogModel {}
    bugTrackerBaseUrl: "https://github.com/Huessenbergnetz/Fuoten/issues/"
}
