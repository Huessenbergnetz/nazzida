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
    property date day
    property DailyLiquidListFilterModel dailyLiquidsModel: null

    Component.onCompleted: liquidsModel.loadForPerson(person.id, person.dayStarts, day)

    SilicaListView {
        id: liquidsListView
        anchors.fill: parent
        spacing: Theme.paddingMedium

        PullDownMenu {
            MenuItem {
                //: Dialog title and pull down menu entry, means liquids
                //% "Add In‐ or Output"
                text: qsTrId("naz-add-input-output")
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("AddLiquid.qml"), {person: liquidsPage.person, liquidsModel: liquidsModel, dailyLiquidsModel: liquidsPage.dailyLiquidsModel})
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
            //: %1 will be the first name, %2 the second name, %3 will be the date in short format, %4 will be the liquid amount in ml
            //% "%1 %2, %3, %4 ml"
            description: qsTrId("naz-name-day-total").arg(person.firstName).arg(person.lastName).arg(liquidsPage.day.toLocaleDateString(Qt.locale(), Locale.ShortFormat)).arg((liquidsModel.difference - person.transpire < 0 ? "" : "+") + Number(liquidsModel.difference - person.transpire).toLocaleString(Qt.locale(), 'f', 0))
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

                Text {
                    text: Qt.formatDateTime(model.moment)
                    color: liquidItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    width: parent.width
                    font.pixelSize: Theme.fontSizeSmall
                }
            }

            RemorseItem {
                id: removeRemorse
            }

            function removeLiquid() {
                var idx = index;
                //: remorse item text, %1 will be the liquid amout, %2 the liquid name,
                //: %3 will be the datetime the liquid has been consumed or excreted
                //% "Delete %1 ml %2 from %3"
                removeRemorse.execute(liquidItem, qsTrId("naz-remorse-delete-liquid").arg(Number(model.amount).toLocaleString(Qt.locale())).arg(model.name).arg(Qt.formatDate(model.moment)), function() {
                    var _moment = model.moment
                    var _inOrOut = model.inOrOut
                    var _amount = model.amount
                    if (liquidsModel.remove(liquidsModel.index(idx, 0))) {
                        if (dailyLiquidsModel) {
                            dailyLiquidsModel.remove(_moment, _inOrOut, _amount)
                        }
                        if (liquidsListView.count == 0) {
                            pageStack.pop()
                        }
                    }
                })
            }

            menu: ContextMenu {
                MenuItem {
                    text: qsTrId("naz-contextmenu-edit")
                    onClicked: pageStack.animatorPush(Qt.resolvedUrl("EditLiquid.qml"), {person: liquidsPage.person, liquidsModel: liquidsModel, dailyLiquidsModel: liquidsPage.dailyLiquidsModel, modelIndex: index, moment: model.moment, inOrOut: model.inOrOut, amount: model.amount, name: model.name, note: model.note})
                }

                MenuItem {
                    text: qsTrId("naz-contextmenu-del")
                    onClicked: removeLiquid()
                }
            }
        }
    }
}
