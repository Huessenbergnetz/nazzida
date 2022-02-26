# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: GPL-3.0-or-later

include(FindPkgConfig)
pkg_search_module(Sailfish sailfishapp REQUIRED IMPORTED_TARGET)

add_executable(nazzida_exe)

target_sources(nazzida_exe
    PRIVATE
        sfosconfig.h
        sfosconfig.cpp
        main.cpp
)

add_subdirectory(models)
add_subdirectory(../../common common)

GNUInstallDirs_get_absolute_install_dir(NAZZIDA_EXE_RPATH CMAKE_INSTALL_LIBDIR)

set_property(TARGET nazzida_exe
    APPEND PROPERTY INSTALL_RPATH "${NAZZIDA_EXE_RPATH}"
)

set_property(TARGET nazzida_exe
    PROPERTY OUTPUT_NAME "${NAZZIDA_EXE_OUTPUT_NAME}"
)

set(CMAKE_SKIP_RPATH OFF CACHE BOOL "" FORCE)

GNUInstallDirs_get_absolute_install_dir(NAZZIDA_FULL_I18NDIR NAZZIDA_I18NDIR)

target_compile_definitions(nazzida_exe PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    $<$<NOT:$<CONFIG:Debug>>:QT_NO_DEBUG_OUTPUT>
    NAZZIDA_VERSION="${PROJECT_VERSION}"
    NAZZIDA_I18NDIR="${NAZZIDA_FULL_I18NDIR}"
)

target_link_libraries(nazzida_exe
    PRIVATE
        Qt5::Core
        Qt5::Quick
        Qt5::Sql
        PkgConfig::Sailfish
        HbnSfosComponentsQt5::Core
        FirfuoridaQt5::Core
)

install(TARGETS nazzida_exe
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
