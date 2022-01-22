set(QMLSRCS
    harbour-nazzida.qml
    cover/CoverPage.qml
    pages/FirstPage.qml
    pages/SecondPage.qml
)

add_custom_target(qmlfiles
    SOURCES ${QMLSRCS}
)

install(FILES harbour-nazzida.qml DESTINATION ${CMAKE_INSTALL_DATADIR}/harbour-nazzida/qml)
install(DIRECTORY cover pages DESTINATION ${CMAKE_INSTALL_DATADIR}/harbour-nazzida/qml)
