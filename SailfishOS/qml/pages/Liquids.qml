/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0

Page {
    id: liquidsPage
    allowedOrientations: defaultAllowedOrientations
    property Person person: null

    Component.onCompleted: liquidsModel.loadForPerson(person.id)

    SilicaListView {
        anchors.fill: parent
        spacing: Theme.paddingMedium

        PullDownMenu {
            MenuItem {
                //: Dialog title and pull down menu entry, means liquids
                //% "Add In‐ or Output"
                text: qsTrId("naz-add-input-output")
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("AddLiquid.qml"), {person: liquidsPage.person, liquidsModel: liquidsModel})
            }
        }

        VerticalScrollDecorator {
            flickable: parent
            page: liquidsPage
        }

        model: LiquidListFilterModel {
            id: liquidsModel
            sortOrder: Qt.DescendingOrder
        }

        header: PageHeader {
            title: qsTrId("naz-fluid-balance-protocol")
            description: qsTrId("naz-full-name").arg(person.firstName).arg(person.lastName)
        }

        delegate: ListItem {
            id: liquidItem
            contentHeight: Theme.itemSizeMedium
            ListView.onRemove: animateRemoval(liquidItem)

            ListView.onAdd: AddAnimation {
                target: liquidItem
            }

            Icon {
                id: inOutIcon
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                }
                source: model.inOrOut === Liquid.In ? "image://nazzida/icon-l-cup" : model.inOrOut === Liquid.InIV ? "image://nazzida/icon-l-syringe" : "image://nazzida/icon-l-peeing"
                width: Theme.iconSizeLarge
                height: Theme.iconSizeLarge
                highlighted: liquidItem.highlighted
            }

            Column {
                anchors {
                    left: inOutIcon.right
                    leftMargin: Theme.paddingLarge
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }

                Label {
                    //: %1 will be the amount of the liquid, %2 will be the liquid name
                    //% "%1 ml %2"
                    text: qsTrId("naz-full-liquid-title").arg(Number(model.amount).toLocaleString(Qt.locale())).arg(model.name)
                    color: liquidItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                    width: parent.width
                }
            }
        }
    }
}
