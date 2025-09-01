# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: GPL-3.0-or-later

target_sources(nazzida_exe
    PRIVATE
        configuration.h
        configuration.cpp
        nazzidautils.h
        nazzidautils.cpp
)

target_include_directories(nazzida_exe
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(migrations)
add_subdirectory(models)
add_subdirectory(objects)
