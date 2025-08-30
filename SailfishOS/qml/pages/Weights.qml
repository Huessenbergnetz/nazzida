/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0

Page {
    id: weightsPage
    allowedOrientations: defaultAllowedOrientations
    property Person person: null

    Component.onCompleted: weightsModel.loadForPerson(person.id)

    SilicaListView {
        id: weightsListView
        anchors.fill: parent
        spacing: Theme.paddingMedium

        PullDownMenu {
            MenuItem {
                //: Dialog title and pull down menu entry
                //% "Add weight"
                text: qsTrId("naz-add-weight")
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("AddWeight.qml"), {person: weightsPage.person, weightsModel: weightsModel})
            }
        }

        VerticalScrollDecorator {
            flickable: parent
            page: weightsPage
        }

        model: WeightListFilterModel {
            id: weightsModel
            sortOrder: Qt.DescendingOrder
        }

        header: PageHeader {
            title: qsTrId("naz-weight-protocol")
            description: qsTrId("naz-full-name").arg(person.firstName).arg(person.lastName)
        }

        delegate: ListItem {
            id: weightItem
            contentHeight: Theme.itemSizeMedium
            ListView.onRemove: animateRemoval(weightItem)
            ListView.onAdd: AddAnimation {
                target: weightItem
            }

            Icon {
                id: weightIcon
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                }
                source: "image://nazzida/icon-l-scale"
                width: Theme.iconSizeLarge
                height: Theme.iconSizeLarge
                highlighted: weightItem.highlighted
            }

            Column {
                anchors {
                    left: weightIcon.right
                    leftMargin: Theme.paddingLarge
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }

                Label {
                    id: weightText
                    //: weight with unit, %1 will be the weight like 70.3
                    //: and %2 will be the unit like kg
                    //% "%1 %2"
                    text: qsTrId("naz-weight-with-unit").arg(Number(model.weight / 1000.0).toLocaleString(Qt.locale(), 'f', 1)).arg("kg")
                    color: weightItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                    width: parent.width
                }

                Text {
                    id: weightMomentText
                    text: Qt.formatDateTime(model.moment)
                    color: weightItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    width: parent.width
                    font.pixelSize: Theme.fontSizeSmall
                }
            }

            RemorseItem {
                id: removeRemorse
            }

            function removeWeight() {
                var idx = index;
                //: remorse item text, %1 will be the weight amount like 70.5 kg,
                //: %2 will be the datetime the weight has been added
                //% "Delete %1 from %2"
                removeRemorse.execute(weightItem, qsTrId("naz-remorse-delete-weight").arg(weightText.text).arg("kg").arg(weightMomentText.text), function() {
                    weightsModel.remove(weightsModel.index(idx, 0))
                })
            }

            menu: ContextMenu {
                MenuItem {
                    //: context menu item text
                    //% "Edit"
                    text: qsTrId("naz-contextmenu-edit")
                }

                MenuItem {
                    //: context menu item text
                    //% "Delete"
                    text: qsTrId("naz-contextmenu-del")
                    onClicked: removeWeight()
                }
            }
        }

        ViewPlaceholder {
            enabled: weightsListView.count === 0
            //: View placeholder text
            //% "No weights added"
            text: qsTrId("naz-weights-list-empty-text")
            //: View placeholder hint
            //% "Pull down to add weights"
            hintText: qsTrId("naz-weights-list-empty-hint")
        }
    }
}
