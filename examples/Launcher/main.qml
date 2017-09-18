import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.VirtualKeyboard 2.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

import ivi.launcher 1.0

ApplicationWindow {
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    visible: true

    ApplicationManager {
        id: applicationManager
    }

    Item {
        anchors.centerIn: parent
        rotation: (parent.width < parent.height)? 0 : -90
        width: (rotation == 0)? parent.width : parent.height
        height: (rotation == 0)? parent.height : parent.width

        Item {
            anchors.fill: parent

            LinearGradient {
                id: background
                anchors.fill: parent
                property color topColor: "khaki"
                SequentialAnimation on topColor {
                    running: true
                    loops: Animation.Infinite
                    ColorAnimation { easing.type: Easing.InOutExpo; easing.amplitude: 1.2; to: "gold"; duration: 8000 }
                    ColorAnimation { easing.type: Easing.InOutExpo; easing.amplitude: 1.2; to: "khaki"; duration: 8000 }
                }

                start: Qt.point(0, 0)
                end: Qt.point(0, height/3)
                gradient: Gradient {
                    GradientStop { position: 0; color: background.topColor }
                    GradientStop { position: 1; color: "goldenrod" }
                }
            }

            Column {
                anchors.centerIn: parent
                Text {
                    id: titleHeader
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 20
                    text: "De super mooie:"
                }
                Text {
                    id: title
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 50
                    text: "LAYR Launcher"
                }
            }
        }

        Column {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: keyboard.top
            property real containerHeight: height / Math.max(appRepeater.count, 1)

            Repeater {
                id: appRepeater
                model: applicationManager.applications

                Rectangle {
                    width: parent.width
                    height: parent.containerHeight
                    color: (index % 2 == 0) ? "cornflowerblue" : "burlywood"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 30
                        text: "App " + (index + 1) + "\nName: " + model.modelData.name
                    }
                    AppContainer {
                        anchors.fill: parent
                        application: model.modelData

                        Component.onCompleted: {
                            application.launch()
                        }
                    }
                }
            }
        }

        InputPanel {
            id: keyboard
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.bottom
            anchors.topMargin: active? -height : 0
            Behavior on anchors.topMargin { PropertyAnimation { easing.type: Easing.InOutQuad } }
        }
    }
}
