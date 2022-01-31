# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: GPL-3.0-or-later

if (${TARGETSYSTEM} STREQUAL "sfos")
    add_subdirectory(libfirfuorida)
    add_subdirectory(HBN_SFOS_Cmake)
    add_subdirectory(HBN_SFOS_Components)
endif(${TARGETSYSTEM} STREQUAL "sfos")
