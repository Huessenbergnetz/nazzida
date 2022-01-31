# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: GPL-3.0-or-later

add_custom_target(qmlfiles
    SOURCES
    main.qml
)

install(FILES main.qml DESTINATION ${CMAKE_INSTALL_DATADIR}/nazzida/qml)
#install(DIRECTORY cover pages models ../../common/qml/models DESTINATION ${CMAKE_INSTALL_DATADIR}/harbour-nazzida/qml)
