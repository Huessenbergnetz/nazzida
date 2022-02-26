# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: GPL-3.0-or-later

set(commonqml_SRCS
    models/ContributorsModel.qml
    models/PrivacyPolicyModel.qml
)

add_custom_target(commonqml
    SOURCES ${commonqml_SRCS}
)

install(DIRECTORY models DESTINATION ${NAZZIDA_QMLDIR})
