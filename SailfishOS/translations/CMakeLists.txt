# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: GPL-3.0-or-later

include(HBNSfosGenerateTranslations)

set(TS_FILES
    nazzida.ts
    nazzida_de.ts
    nazzida_en_GB.ts
    nazzida_en_US.ts)

hbn_sfos_add_translation(QM_FILES ${TS_FILES} OPTIONS -idbased)

add_custom_target(nazzidatrans ALL
                  DEPENDS ${QM_FILES}
                  COMMENT "Compiles nazzida translation messsages"
                  SOURCES ${TS_FILES})

install(FILES ${QM_FILES}
        DESTINATION ${NAZZIDA_TRANSDIR}
        COMPONENT runtime)
