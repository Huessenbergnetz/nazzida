add_executable(harbour-nazzida main.cpp)

target_compile_definitions(harbour-nazzida PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    NAZZIDA_VERSION="${PROJECT_VERSION}"
)

target_include_directories(harbour-nazzida PRIVATE
    $<BUILD_INTERFACE:
    ${SAILFISH_INCLUDE_DIRS}
>)

target_link_libraries(harbour-nazzida
    Qt5::Core
    Qt5::Quick
    Qt5::Network
    Qt5::Sql
    ${SAILFISH_LDFLAGS}
)

install(TARGETS harbour-nazzida
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
