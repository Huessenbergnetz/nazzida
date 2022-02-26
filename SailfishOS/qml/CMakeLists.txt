# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: GPL-3.0-or-later

set(QMLSRCS
    harbour-nazzida.qml
    cover/CoverPage.qml
    pages/FirstPage.qml
    pages/AddPerson.qml
    pages/EditPerson.qml
    pages/About.qml
    pages/PrivacyPolicy.qml
    pages/Settings.qml
    pages/Sections.qml
    pages/DailyLiquids.qml
    pages/Liquids.qml
    pages/AddLiquid.qml
    pages/EditLiquid.qml
    models/ChangelogModel.qml
)

add_custom_target(qmlfiles
    SOURCES ${QMLSRCS}
)

install(FILES harbour-nazzida.qml DESTINATION ${NAZZIDA_QMLDIR})
install(DIRECTORY cover pages models DESTINATION ${NAZZIDA_QMLDIR})
