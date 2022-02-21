/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0

Page {
    id: dailyLiquidsPage
    allowedOrientations: defaultAllowedOrientations
    property Person person: null

    Component.onCompleted: dailyLiquidsModel.loadForPerson(person.id, person.dayStarts)

    SilicaListView {
        id: dailyLiquidsView
        anchors.fill: parent
        spacing: Theme.paddingMedium

        PullDownMenu {
            MenuItem {
                text: qsTrId("naz-add-input-output")
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("AddLiquid.qml"), {person: dailyLiquidsPage.person, dailyLiquidsModel: dailyLiquidsModel})
            }
        }

        VerticalScrollDecorator {
            flickable: parent
            page: dailyLiquidsPage
        }

        model: DailyLiquidListFilterModel {
            id: dailyLiquidsModel
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

            onClicked: pageStack.animatorPush(Qt.resolvedUrl("Liquids.qml"), {person: dailyLiquidsPage.person, day: model.day, dailyLiquidsModel: dailyLiquidsModel})

            Icon {
                id: inOutIcon
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                }
                source: "image://theme/icon-l-transfer"
                width: Theme.iconSizeLarge
                height: Theme.iconSizeLarge
                highlighted: liquidItem.highlighted
                rotation: model.difference - person.transpire > 0 ? 180 : 0
            }

            Column {
                anchors {
                    left: inOutIcon.right
                    leftMargin: Theme.paddingLarge
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }

                Label {
                    text: model.day.toLocaleDateString(Qt.locale())
                    color: liquidItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                    width: parent.width
                }

                Row {
                    width: parent.width
                    spacing: Theme.paddingSmall

                    Item {
                        width: parent.width/3
                        height: Theme.iconSizeSmall

                        Icon {
                            id: inputIcon
                            source: "image://theme/icon-m-up"
                            anchors {
                                left: parent.left
                            }
                            width: Theme.iconSizeSmall
                            height: Theme.iconSizeSmall
                            highlighted: liquidItem.highlighted
                        }

                        Text {
                            anchors {
                                left: inputIcon.right
                                right: parent.right
                            }
                            text: "+" + model.input.toLocaleString(Qt.locale(), 'f', 0)
                            color: liquidItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeSmall
                        }
                    }

                    Item {
                        width: parent.width/3
                        height: Theme.iconSizeSmall

                        Icon {
                            id: outputIcon
                            source: "image://theme/icon-m-down"
                            anchors {
                                left: parent.left
                            }

                            width: Theme.iconSizeSmall
                            height: Theme.iconSizeSmall
                            highlighted: liquidItem.highlighted
                        }

                        Text {
                            anchors {
                                left: outputIcon.right
                                right: parent.right
                            }

                            text: "-" + Number(model.output + person.transpire).toLocaleString(Qt.locale(), 'f', 0)
                            color: liquidItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeSmall
                        }
                    }

                    Item {
                        width: parent.width/3
                        height: Theme.iconSizeSmall

                        Icon {
                            id: diffIcon
                            anchors {
                                left: parent.left
                            }
                            source: "image://theme/icon-m-transfer"
                            width: Theme.iconSizeSmall
                            height: Theme.iconSizeSmall
                            highlighted: liquidItem.highlighted
                        }

                        Text {
                            anchors {
                                left: diffIcon.right
                                right: parent.right
                            }
                            text: (model.difference - person.transpire < 0 ? "" : "+") + Number(model.difference - person.transpire).toLocaleString(Qt.locale(), 'f', 0)
                            color: liquidItem.highlighted ? Theme.secondaryHighlightColor : model.difference - person.transpire > 0 ? Theme.errorColor : Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeSmall
                        }
                    }
                }
            }
        }

        ViewPlaceholder {
            enabled: dailyLiquidsView.count === 0
            //: View placeholder text
            //% "No liquids added"
            text: qsTrId("naz-liquids-list-empty-text")
            //: View placeholder hint
            //% "Pull down to add liquids"
            hintText: qsTrId("naz-liquids-list-empty-hint")
        }
    }
}
