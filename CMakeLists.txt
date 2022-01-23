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

if (${TARGETSYSTEM} STREQUAL "sfos")
    add_subdirectory(3rdparty)
    add_subdirectory(SailfishOS)

    add_custom_target(specfiles
        SOURCES
            rpm/harbour-nazzida.changes.in
            rpm/harbour-nazzida.changes.run.in
            rpm/harbour-nazzida.spec
            rpm/harbour-nazzida.yaml
    )
endif (${TARGETSYSTEM} STREQUAL "sfos")
