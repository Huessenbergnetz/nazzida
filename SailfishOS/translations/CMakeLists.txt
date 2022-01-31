# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: GPL-3.0-or-later

include(HBNSfosGenerateTranslations)

hbn_sfos_gen_translation(nazzidatrans IDBASED INSTALL_DESTINATION ${NAZZIDA_TRANSDIR} INPUT_FILE nazzida.ts)

set(_langs de)
foreach(_lang ${_langs})
    hbn_sfos_gen_translation(nazzidatrans IDBASED INSTALL_DESTINATION ${NAZZIDA_TRANSDIR} INPUT_FILE nazzida_${_lang}.ts)
endforeach()
