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

            property double bmi: NazzidaUtils.calcBmi(weightsPage.person, model.weight)
            property int bmiDetailed: NazzidaUtils.bmiDetailed(weightsPage.person, bmi)
            property bool isAdult: NazzidaUtils.ageAtMoment(weightsPage.person, model.moment) > 18

            contentHeight: Theme.itemSizeLarge
            ListView.onRemove: animateRemoval(weightItem)
            ListView.onAdd: AddAnimation {
                target: weightItem
            }

            Icon {
                id: weightIcon
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    verticalCenter: parent.verticalCenter
                }
                source: weightItem.isAdult ? "image://nazzida/icon-l-bmimeter-" + weightItem.bmiDetailed.toString() : "image://nazzida/icon-l-scale"
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
                    verticalCenter: parent.verticalCenter
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

                Text {
                    id: bmiText
                    visible: weightItem.isAdult
                    //: %1 will be replaced by the body mass index like 22.1,
                    //: %2 will be replaced by a BMI classification like Normal weight
                    //: or Pre-obese
                    //% "BMI: %1 (%2)"
                    text: qsTrId("naz-weight-bmi").arg(Number(weightItem.bmi).toLocaleString(Qt.locale(), 'f', 1)).arg(NazzidaUtils.bmiDetailedString(weightsPage.person, weightItem.bmi))
                    color: weightItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    width: parent.width
                    font.pixelSize: Theme.fontSizeSmall
                }
            }

            menu: ContextMenu {
                MenuItem {
                    //: context menu item text
                    //% "Edit"
                    text: qsTrId("naz-contextmenu-edit")
                    onClicked: pageStack.animatorPush(Qt.resolvedUrl("EditWeight.qml"),
                                                      {person: weightsPage.person,
                                                       weightModel: weightsModel,
                                                       modelIndex: index,
                                                       moment: model.moment,
                                                       weight: model.weight,
                                                       note: model.note}
                                                      )
                }

                MenuItem {
                    //: context menu item text
                    //% "Delete"
                    text: qsTrId("naz-contextmenu-del")
                    onClicked: weightItem.remorseDelete(function() {
                        weightsModel.remove(weightsModel.index(index, 0))
                    })
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
