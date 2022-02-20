# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: GPL-3.0-or-later

include(FindPkgConfig)
pkg_search_module(Sailfish sailfishapp REQUIRED IMPORTED_TARGET)

add_executable(harbour-nazzida
    ../../common/src/configuration.h
    ../../common/src/configuration.cpp
    ../../common/src/migrations/m20220127t134808_people.h
    ../../common/src/migrations/m20220127t134808_people.cpp
    ../../common/src/migrations/m20220130t123658_liquid.h
    ../../common/src/migrations/m20220130t123658_liquid.cpp
    ../../common/src/migrations/m20220218t081651_people_transpire.h
    ../../common/src/migrations/m20220218t081651_people_transpire.cpp
    ../../common/src/objects/person.h
    ../../common/src/objects/person.cpp
    ../../common/src/objects/liquid.h
    ../../common/src/objects/liquid_p.h
    ../../common/src/objects/liquid.cpp
    ../../common/src/objects/dailyliquids.h
    ../../common/src/objects/dailyliquids_p.h
    ../../common/src/objects/dailyliquids.cpp
    ../../common/src/models/baselistmodel.h
    ../../common/src/models/baselistmodel.cpp
    ../../common/src/models/basefiltermodel.h
    ../../common/src/models/basefiltermodel.cpp
    ../../common/src/models/peoplelistmodel.h
    ../../common/src/models/peoplelistmodel.cpp
    ../../common/src/models/peoplelistfiltermodel.h
    ../../common/src/models/peoplelistfiltermodel.cpp
    ../../common/src/models/liquidlistmodel.h
    ../../common/src/models/liquidlistmodel.cpp
    ../../common/src/models/liquidlistfiltermodel.h
    ../../common/src/models/liquidlistfiltermodel.cpp
    ../../common/src/models/dailyliquidlistmodel.h
    ../../common/src/models/dailyliquidlistmodel.cpp
    ../../common/src/models/dailyliquidlistfiltermodel.h
    ../../common/src/models/dailyliquidlistfiltermodel.cpp
    ../../common/src/models/languagesmodel.h
    ../../common/src/models/languagesmodel.cpp
    sfosconfig.h
    sfosconfig.cpp
    models/licensesmodel.h
    models/licensesmodel.cpp
    main.cpp
)

set_property(TARGET harbour-nazzida
    APPEND PROPERTY INSTALL_RPATH "/usr/share/harbour-nazzida/lib"
)

set(CMAKE_SKIP_RPATH OFF CACHE BOOL "" FORCE)

target_compile_definitions(harbour-nazzida PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    $<$<NOT:$<CONFIG:Debug>>:QT_NO_DEBUG_OUTPUT>
    NAZZIDA_VERSION="${PROJECT_VERSION}"
    NAZZIDA_I18NDIR="/usr/share/harbour-nazzida/translations"
)

target_link_libraries(harbour-nazzida
    PRIVATE
        Qt5::Core
        Qt5::Quick
        Qt5::Sql
        PkgConfig::Sailfish
        HbnSfosComponentsQt5::Core
        FirfuoridaQt5::Core
)

install(TARGETS harbour-nazzida
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
