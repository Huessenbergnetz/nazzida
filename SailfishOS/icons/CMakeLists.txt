# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: GPL-3.0-or-later

include(HBNSfosGenerateIcons)

hbn_sfos_gen_icons(genicons INPUT_FILE harbour-nazzida.svg APPICON)

hbn_sfos_gen_icons(genicons INPUT_FILE cup.svg SIZES "l")
hbn_sfos_gen_icons(genicons INPUT_FILE peeing.svg SIZES "l")
hbn_sfos_gen_icons(genicons INPUT_FILE syringe.svg SIZES "l")
