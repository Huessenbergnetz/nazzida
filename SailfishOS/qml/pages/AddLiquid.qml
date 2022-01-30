/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.nazzida 1.0


Dialog {
    id: addLiquidDialog
    allowedOrientations: defaultAllowedOrientations
    property Person person: null
    property LiquidListFilterModel liquidsModel: null
    property date moment: new Date()

    SilicaFlickable {
        id: addLiquidFlick
        anchors.fill: parent
        contentHeight: addLiquidCol.height

        VerticalScrollDecorator {
            flickable: parent
            page: addLiquidDialog
        }

        Column {
            id: addLiquidCol
            width: parent.width

            DialogHeader {
                flickable: addLiquidFlick
                title: qsTrId("naz-add-input-output")
                //: Accept text for add in- or output dialog
                //% "Add"
                acceptText: qsTrId("naz-accept-add-inoutput")
            }

            Item {
                width: parent.width
                height: Math.max(inOutIcon.height, typePicker.height)

                Icon {
                    id: inOutIcon
                    anchors {
                        left: parent.left
                        leftMargin: Theme.horizontalPageMargin
                    }
                    source: typePicker.currentItem.value === Liquid.In ? "image://nazzida/icon-l-cup" : typePicker.currentItem.value === Liquid.InIV ? "image://nazzida/icon-l-syringe" : "image://nazzida/icon-l-peeing"
                    width: Theme.iconSizeLarge
                    height: Theme.iconSizeLarge
                }

                ComboBox {
                    id: typePicker
                    anchors {
                        left: inOutIcon.right
                        leftMargin: Theme.paddingLarge
                        right: parent.right
                        rightMargin: Theme.horizontalPageMargin
                    }
                    //: label for a combobox on th add liquid dialog
                    //% "In‐ or Output"
                    label: qsTrId("naz-combo-label-inouttype")
                    menu: ContextMenu {
                        MenuItem {
                            //: ComboBox item
                            //% "Drinks/Soups"
                            text: qsTrId("naz-combo-inouttype-item-in")
                            readonly property int value: Liquid.In
                        }
                        MenuItem {
                            //: ComboBox item
                            //% "Intravenous"
                            text: qsTrId("naz-combo-inouttype-item-iniv")
                            readonly property int value: Liquid.InIV
                        }
                        MenuItem {
                            //: ComboBox item, excretion of body fluids
                            //% "Excretion"
                            text: qsTrId("naz-combo-inouttype-item-out")
                            readonly property int value: Liquid.Out
                        }
                    }
                }
            }
        }
    }
}
