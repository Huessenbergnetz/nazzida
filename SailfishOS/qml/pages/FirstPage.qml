/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0

Page {
    id: firstPage
    allowedOrientations: Orientation.All

    Component.onCompleted: peopleListModel.load()

    SilicaListView {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                //: Page title and pull down menu entry
                //% "About"
                text: qsTrId("naz-title-about")
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("About.qml"))
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

        model: PeopleListModel {
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

            Column {
                anchors {
                    left: parent.left
                    right: parent.right
                    leftMargin: Theme.horizontalPageMargin
                    rightMargin: Theme.horizontalPageMargin
                }

                Label {
                    //: %1 will be the first, %2 the last name
                    //% "%1 %2"
                    text: qsTrId("naz-full-name").arg(model.firstName).arg(model.lastName)
                    color: peopleListItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                    width: parent.width
                }

                Text {

                }
            }
        }
    }
}
