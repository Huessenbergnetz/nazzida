set(QMLSRCS
    harbour-nazzida.qml
    cover/CoverPage.qml
    pages/FirstPage.qml
    pages/AddPerson.qml
    pages/EditPerson.qml
    pages/About.qml
    models/ChangelogModel.qml
    ../../common/qml/models/ContributorsModel.qml
)

add_custom_target(qmlfiles
    SOURCES ${QMLSRCS}
)

install(FILES harbour-nazzida.qml DESTINATION ${CMAKE_INSTALL_DATADIR}/harbour-nazzida/qml)
install(DIRECTORY cover pages models ../../common/qml/models DESTINATION ${CMAKE_INSTALL_DATADIR}/harbour-nazzida/qml)
