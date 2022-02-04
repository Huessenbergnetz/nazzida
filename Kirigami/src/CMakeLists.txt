# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: GPL-3.0-or-later

add_executable(nazzida
    ../../common/src/migrations/m20220127t134808_people.h
    ../../common/src/migrations/m20220127t134808_people.cpp
    ../../common/src/migrations/m20220130t123658_liquid.h
    ../../common/src/migrations/m20220130t123658_liquid.cpp
    main.cpp
    )

target_compile_features(nazzida PRIVATE cxx_std_14)

target_compile_definitions(nazzida PRIVATE
    QT_NO_CAST_TO_ASCII
    QT_NO_CAST_FROM_ASCII
    QT_STRICT_ITERATORS
    QT_NO_URL_CAST_FROM_STRING
    QT_NO_CAST_FROM_BYTEARRAY
    QT_USE_QSTRINGBUILDER
    QT_USE_FAST_OPERATOR_PLUS
    QT_DISABLE_DEPRECATED_BEFORE=0x050900
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    $<$<NOT:$<CONFIG:Debug>>:QT_NO_DEBUG_OUTPUT>
    NAZZIDA_VERSION="${PROJECT_VERSION}"
    $<BUILD_INTERFACE:NAZZIDA_QML_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../qml">
    $<INSTALL_INTERFACE:NAZZIDA_QML_DIR="${CMAKE_INSTALL_DATADIR}/nazzida/qml">
)

target_link_libraries(nazzida
    PRIVATE
        Qt5::Quick
        Qt5::Qml
        Qt5::Gui
        Qt5::QuickControls2
        Qt5::Widgets
        Qt5::Network
        Qt5::Sql
        KF5::Kirigami2
        KF5::I18n
        Firfuorida::Firfuorida
)

install(TARGETS nazzida
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
