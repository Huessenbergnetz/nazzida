# SPDX-FileCopyrightText: (C) 2022-2025 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: GPL-3.0-or-later

include(HBNSfosGenerateIcons)

hbn_sfos_add_appicon(appIcons harbour-nazzida.svg)

add_custom_target(nazzidaAppIcon ALL
                  DEPENDS ${appIcons}
                  COMMENT "Generating application icons"
                  SOURCES harbour-nazzida.svg)

set(L_ICONS
    cup.svg
    peeing.svg
    syringe.svg
    scale.svg)

hbn_sfos_add_icon(lIcons ${L_ICONS} SIZES "l")

add_custom_target(largeIcons ALL
                  DEPENDS ${lIcons}
                  COMMENT "Generating icons"
                  SOURCES ${L_ICONS})
