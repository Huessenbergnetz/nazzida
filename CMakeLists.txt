cmake_minimum_required(VERSION 3.12.4)
cmake_policy(SET CMP0048 NEW)

project(nazzida
    VERSION 0.0.1
    DESCRIPTION "Personal Fluid Balance Protocol"
    HOMEPAGE_URL "https://github.com/Huessenbergnetz/nazzida"
    LANGUAGES CXX)

find_package (Qt5 COMPONENTS Core Network Qml Gui Quick Sql REQUIRED)

include(FindPkgConfig)
include(GNUInstallDirs)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(TARGETSYSTEM "sfos" CACHE STRING "Target system to build the application for, currently supported: sfos")
set(NAZZIDA_TRANSDIR "${CMAKE_INSTALL_DATADIR}/nazzida/translations" CACHE PATH "Installation destination for translation files.")

if (${TARGETSYSTEM} STREQUAL "sfos")
    set(HARBOUR_APPNAME harbour-nazzida)
    set(CMAKE_INSTALL_LIBDIR share/${HARBOUR_APPNAME}/lib)
    set(HBNSC_WITH_FIRFUORIDA On)
    set(HBNSC_WITH_OPENSSL On)
    set(HBNSC_LICENSES "GPLv3;LGPLv2_1;BSD-3;CC-BY-4_0;OpenSSL")
    set(NAZZIDA_TRANSDIR share/${HARBOUR_APPNAME}/translations)

    list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/3rdparty/HBN_SFOS_Cmake)

    add_subdirectory(3rdparty)
    add_subdirectory(SailfishOS)
    add_subdirectory(translations)

    add_custom_target(specfiles
        SOURCES
            rpm/harbour-nazzida.changes.in
            rpm/harbour-nazzida.changes.run.in
            rpm/harbour-nazzida.spec
            rpm/harbour-nazzida.yaml
    )
endif (${TARGETSYSTEM} STREQUAL "sfos")
