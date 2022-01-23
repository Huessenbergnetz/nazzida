include(FindPkgConfig)
pkg_search_module(Sailfish sailfishapp REQUIRED IMPORTED_TARGET)

add_executable(harbour-nazzida main.cpp)

target_compile_definitions(harbour-nazzida PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    NAZZIDA_VERSION="${PROJECT_VERSION}"
)

target_link_libraries(harbour-nazzida
    PRIVATE
        Qt5::Core
        Qt5::Quick
        Qt5::Network
        Qt5::Sql
        PkgConfig::Sailfish
)

install(TARGETS harbour-nazzida
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
