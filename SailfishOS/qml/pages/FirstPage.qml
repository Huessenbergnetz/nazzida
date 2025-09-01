/*
 * SPDX-FileCopyrightText: (C) 2022-2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0

Page {
    id: firstPage
    allowedOrientations: defaultAllowedOrientations

    Component.onCompleted: peopleListModel.load()

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: peopleListModel.inOperation
    }

    SilicaListView {
        id: peopleListView
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                //: Page title and pull down menu entry
                //% "About"
                text: qsTrId("naz-title-about")
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("About.qml"))
            }
            MenuItem {
                //: Page title and pull down menu entry
                //% "Settings"
                text: qsTrId("naz-title-settings")
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("Settings.qml"))
            }
            MenuItem {
                //: Page title and pull down menu enty
                //% "Add Person"
                text: qsTrId("naz-title-add-person")
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("AddPerson.qml"), {peopleModel: peopleListModel})
            }
        }

        VerticalScrollDecorator {
            flickable: parent
            page: firstPage
        }

        model: PeopleListFilterModel {
            id: peopleListModel
        }

        header: PageHeader {
            title: "Nazzida"
            //: header page description for first page
            //% "List of logged people"
            description: qsTrId("naz-first-page-header-desc")
        }

        delegate: ListItem {
            id: peopleListItem
            contentHeight: Theme.itemSizeMedium
            ListView.onRemove: animateRemoval(peopleListItem)

            ListView.onAdd: AddAnimation {
                target: peopleListItem
            }

            onClicked: pageStack.animatorPush(Qt.resolvedUrl("Sections.qml"), {person: model.item})

            Icon {
                id: personIcon
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                }
                source: "image://theme/icon-l-people"
                width: Theme.iconSizeLarge
                height: Theme.iconSizeLarge
                highlighted: peopleListItem.highlighted
            }

            Column {
                anchors {
                    left: personIcon.right
                    right: parent.right
                    leftMargin: Theme.paddingLarge
                    rightMargin: Theme.horizontalPageMargin
                }

                Label {
                    //: %1 will be the first, %2 the last name
                    //% "%1 %2"
                    text: qsTrId("naz-full-name").arg(model.firstName).arg(model.lastName)
                    color: peopleListItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                    width: parent.width
                }

                Row {
                    spacing: Theme.paddingMedium

                    Text {
                        color: peopleListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeSmall
                        text: model.sex === "m" ? "♂︎" : model.sex === "f" ? "♀︎" : "⚧︎"
                    }

                    Text {
                        color: peopleListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeSmall
                        //: %1 will be the body size in cm
                        //% "%1 cm"
                        text: qsTrId("naz-size-in-cm").arg(model.size)
                    }

                    Text {
                        color: peopleListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeSmall
                        //: %1 will be the age in years
                        //% "%n year(s)"
                        text: qsTrId("naz-age-in-years", model.age)
                    }
                }
            }

            RemorseItem {
                id: removeRemorse
            }

            function removeItem() {
                var idx = index;
                //: remorse item text, %1 will be the first, %2 the second name
                //% "Delete %1 %2"
                removeRemorse.execute(peopleListItem, qsTrId("naz-remorse-delete").arg(model.firstName).arg(model.lastName), function() { peopleListModel.remove(peopleListModel.index(idx, 0)) } )
            }

            menu: ContextMenu {
                MenuItem {
                    // defined in Weights.qml
                    text: qsTrId("naz-contextmenu-edit")
                    onClicked: pageStack.animatorPush(Qt.resolvedUrl("EditPerson.qml"), {peopleModel: peopleListModel, person: model.item, modelIndex: index})
                }

                MenuItem {
                    // defined in Weights.qml
                    text: qsTrId("naz-contextmenu-del")
                    onClicked: removeItem()
                }
            }
        }

        ViewPlaceholder {
            enabled: peopleListView.count === 0 && !peopleListModel.inOperation
            //: View placeholder text
            //% "No people added"
            text: qsTrId("naz-people-list-empty-text")
            //: View placeholder hint
            //% "Pull down to add people"
            hintText: qsTrId("naz-people-list-empty-hint")
        }
    }
}
