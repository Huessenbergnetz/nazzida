# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: GPL-3.0-or-later

set(QMLSRCS
    harbour-nazzida.qml
    cover/CoverPage.qml
    pages/FirstPage.qml
    pages/AddPerson.qml
    pages/EditPerson.qml
    pages/About.qml
    pages/Settings.qml
    pages/Sections.qml
    pages/Liquids.qml
    pages/AddLiquid.qml
    models/ChangelogModel.qml
    ../../common/qml/models/ContributorsModel.qml
)

add_custom_target(qmlfiles
    SOURCES ${QMLSRCS}
)

install(FILES harbour-nazzida.qml DESTINATION ${CMAKE_INSTALL_DATADIR}/harbour-nazzida/qml)
install(DIRECTORY cover pages models ../../common/qml/models DESTINATION ${CMAKE_INSTALL_DATADIR}/harbour-nazzida/qml)
