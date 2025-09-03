/*
 * SPDX-FileCopyrightText: (C) 2025 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0

Page {
    id: bloodPressurePage
    allowedOrientations: defaultAllowedOrientations
    property Person person: null

    Component.onCompleted: bloodPressureModel.loadForPerson(person.id)

    SilicaListView {
        id: bloodPressureListView
        anchors.fill: parent
        spacing: Theme.paddingLarge

        PullDownMenu {
            MenuItem {
                //: Dialog title and pull down menu entry
                //% "Add blood pressure"
                text: qsTrId("naz-add-blood-pressure")
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("AddBloodPressure.qml"), {person: bloodPressurePage.person, bloodPressureModel: bloodPressureModel})
            }
        }

        VerticalScrollDecorator {
            flickable: parent
            page: bloodPressurePage
        }

        model: BloodPressureListFilterModel {
            id: bloodPressureModel
            sortOrder: Qt.DescendingOrder
        }

        header: PageHeader {
            title: qsTrId("naz-blood-pressure-protocol")
            description: qsTrId("naz-full-name").arg(person.firstName).arg(person.lastName)
        }

        delegate: ListItem {
            id: bloodPressureItem
            contentHeight: Theme.itemSizeLarge
            ListView.onRemove: animateRemoval(bloodPressureItem)
            ListView.onAdd: AddAnimation {
                target: bloodPressureItem
            }

            Icon {
                id: bloodPressureIcon
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    verticalCenter: parent.verticalCenter
                }
                source: "image://nazzida/icon-l-pressure-gauge"
                width: Theme.iconSizeLarge
                height: Theme.iconSizeLarge
                highlighted: bloodPressureItem.highlighted
            }

            Column {
                anchors {
                    left: bloodPressureIcon.right
                    leftMargin: Theme.paddingLarge
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                    verticalCenter: parent.verticalCenter
                }

                Row {
                    width: parent.width
                    spacing: Theme.paddingSmall

                    Label {
                        id: sysDiaText
                        //: list item label showing blood pressure,
                        //: %1 will be the systolisch pressure, %2 the
                        //: diastolic pressure, systolic/diastolic should
                        //: be abbreviated
                        //% "Sys/Dia: %1/%2 mmHg"
                        text: qsTrId("naz-blood-pressure-sysdia-abbr").arg(model.systolic).arg(model.diastolic)
                    }

                    Label {
                        id: pulseText
                        //: list item label schowing blood pulse,
                        //: %1 will be replaced by the pulse value
                        //: pulse should be abbreviated
                        //% "Pul: %1 bpm"
                        text: qsTrId("naz-blood-pressure-pulse-abbr").arg(model.pulse)
                        color: bloodPressureItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                    }
                }

                Text {
                    id: bloodPressureMoment
                    text: Qt.formatDateTime(model.moment)
                    color: bloodPressureItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    width: parent.width
                    font.pixelSize: Theme.fontSizeSmall
                }

                Text {
                    id: bpClassification
                    text: NazzidaUtils.bpClassString(bloodPressurePage.person, config.bpClass, model.systolic, model.diastolic)
                    color: bloodPressureItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    width: parent.width
                    font.pixelSize: Theme.fontSizeSmall
                }
            }

            RemorseItem {
                id: removeRemorse
            }

            function removeBloodPressure() {
                var idx = index;
                //% "Delete %1/%2 from %3"
                removeRemorse.execute(bloodPressureItem, qsTrId("naz-remorse-delete-blood-pressure").arg(model.systolic).arg(model.diastolic).arg(bloodPressureMoment.text), function() {
                    bloodPressureModel.remove(bloodPressureModel.index(idx, 0))
                })
            }

            menu: ContextMenu {
                MenuItem {
                    // defined in Weights.qml
                    text: qsTrId("naz-contextmenu-edit")
                    onClicked: pageStack.animatorPush(Qt.resolvedUrl("EditBloodPressure.qml"),
                                                      {person: bloodPressurePage.person,
                                                       bloodPressureModel: bloodPressureModel,
                                                       modelIndex: index,
                                                       moment: model.moment,
                                                       systolic: model.systolic,
                                                       diastolic: model.diastolic,
                                                       pulse: model.pulse,
                                                       note: model.note}
                                                     )
                }
                MenuItem {
                    // defined in Weights.qml
                    text: qsTrId("naz-contextmenu-del")
                    onClicked: removeBloodPressure()
                }
            }
        }

        section {
            property: "relDate"
            delegate: SectionHeader {
                text: section
                height: Theme.itemSizeExtraSmall
            }
        }

        ViewPlaceholder {
            enabled: bloodPressureListView.count === 0
            //: View placeholder text
            //% "No blood pressure added"
            text: qsTrId("naz-blood-pressure-list-empty-text")
            //: View placeholder hint
            //% "Pull down to add blood pressure"
            hintText: qsTrId("naz-blood-pressure-list-empty-hint")
        }
    }
}
