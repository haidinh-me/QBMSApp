import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../views"

Item {
    id: root

    property var viewModel
    property int currentView: 0

    readonly property real frameAspect: 16 / 9
    readonly property real availableWidth: Math.max(240, root.width - 24)
    readonly property real availableHeight: Math.max(180, root.height - 24)
    readonly property real widthByHeight: availableHeight * frameAspect
    readonly property real fittedWidth: (availableWidth <= widthByHeight) ? availableWidth : widthByHeight
    readonly property real uiScale: parent.width/980

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#efefef" }
            GradientStop { position: 1.0; color: "#d5d5d5" }
        }
    }

    Rectangle {
        id: deviceFrame
        anchors.centerIn: parent
        anchors.fill: parent
        color: "#06080b"
        border.color: "#262a2f"
        border.width: Math.max(1, Math.round(2 * root.uiScale))
        clip: true

        Rectangle {
            anchors.fill: parent
            radius: parent.radius
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#151a22" }
                GradientStop { position: 1.0; color: "#070a10" }
            }
        }

        Rectangle {
            width: parent.width * 0.24
            height: parent.height * 1.5
            rotation: 24
            x: parent.width * 0.32
            y: -parent.height * 0.2
            color: "#25ffffff"
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 12 * root.uiScale
            spacing: 8 * root.uiScale

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 42 * root.uiScale
                radius: 7 * root.uiScale
                color: "#262b31"
                border.color: "#4f565d"

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 8 * root.uiScale

                    Label {
                        text: "BMS Display"
                        color: "#a6f7c8"
                        font.bold: true
                        font.pixelSize: 16 * root.uiScale
                    }

                    Label{
                        text: "[Hải Định] [Tấn Phát] [Tấn Đạt] [Công Phúc] [Minh Thông]"
                        color: "#e0b507"
                        font.bold: true;
                        font.pixelSize: 17 * root.uiScale
                        Layout.alignment: Qt.AlignCenter
                    }

                    Label {
                        text: root.viewModel ? root.viewModel.connectionStateText.toUpperCase() : "IDLE"
                        color: {
                            if (!root.viewModel) {
                                return "#9aa3ad"
                            }
                            if (root.viewModel.connectionStateText === "Online") {
                                return "#2af764"
                            }
                            if (root.viewModel.connectionStateText === "Connecting") {
                                return "#facc15"
                            }
                            if (root.viewModel.connectionStateText === "Degraded") {
                                return "#f97316"
                            }
                            return "#ef4444"
                        }
                        font.bold: true
                        font.pixelSize: 13 * root.uiScale
                    }
                }
            }

            StackLayout {
                id: pageStack
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: root.currentView

                DashboardView {
                    viewModel: root.viewModel
                }

                PacksView {
                    viewModel: root.viewModel
                }

                SettingsView {
                    viewModel: root.viewModel
                }
            }

            BottomTabBar {
                Layout.fillWidth: true
                Layout.preferredHeight: 50 * root.uiScale
                tabs: ["GROUP", "PACKS", "SETTING"]
                currentIndex: root.currentView
                fontScale: root.uiScale
                onTabSelected: function(index) {
                    root.currentView = index
                }
            }
        }
    }
}
