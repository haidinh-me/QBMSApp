import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    id: root

    property var viewModel
    readonly property bool onlineLike: viewModel && (viewModel.connectionStateText === "Online" || viewModel.connectionStateText === "Degraded")
    readonly property bool compactMode: width < 850
    readonly property real uiScale: parent.width/850

    Component.onCompleted: {
        if (viewModel) {
            viewModel.reloadAvailablePorts()
        }
    }

    AppCard {
        anchors.fill: parent
        radius: 8 * root.uiScale
        padding: 10 * root.uiScale

        ColumnLayout {
            anchors.fill: parent
            spacing: 8 * root.uiScale

            Label {
                text: "SETTING"
                color: "#7ecaf2"
                font.bold: true
                font.pixelSize: 14 * root.uiScale
            }

            AppCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 86 * root.uiScale
                radius: 8 * root.uiScale
                backgroundColor: "#171b1f"

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 4 * root.uiScale

                    Label {
                        text: "RS485 Protocol No."
                        color: "#b8c1cb"
                        font.pixelSize: 12 * root.uiScale
                    }

                    TextField {
                        Layout.fillWidth: true
                        readOnly: true
                        text: "000 - 4G-GPS Remote module protocol V4.2"
                        font.pixelSize: 12 * root.uiScale
                    }
                }
            }

            AppCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 86 * root.uiScale
                radius: 8 * root.uiScale
                backgroundColor: "#171b1f"

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 4 * root.uiScale

                    Label {
                        text: "CAN Protocol No."
                        color: "#b8c1cb"
                        font.pixelSize: 12 * root.uiScale
                    }

                    TextField {
                        Layout.fillWidth: true
                        readOnly: true
                        text: "000 - JK BMS CAN Protocol (250K) V2.0"
                        font.pixelSize: 12 * root.uiScale
                    }
                }
            }

            AppCard {
                Layout.fillWidth: true
                Layout.fillHeight: true
                radius: 8 * root.uiScale
                backgroundColor: "#171b1f"

                GridLayout {
                    anchors.fill: parent
                    columns: root.compactMode ? 2 : 4
                    rowSpacing: 8 * root.uiScale
                    columnSpacing: 8 * root.uiScale

                    Label { text: "Port"; color: "#b8c1cb"; font.pixelSize: 12 * root.uiScale }
                    TextField {
                        id: portField
                        Layout.fillWidth: true
                        Layout.minimumWidth: 0
                        text: viewModel ? viewModel.portName : ""
                        placeholderText: "/dev/ttyAMA0"
                        font.pixelSize: 12 * root.uiScale
                    }

                    Label { text: "Detected"; color: "#b8c1cb"; font.pixelSize: 12 * root.uiScale }
                    ComboBox {
                        Layout.fillWidth: true
                        Layout.minimumWidth: 0
                        model: viewModel ? viewModel.availablePorts : []
                        font.pixelSize: 12 * root.uiScale
                        onActivated: {
                            portField.text = currentText
                        }
                    }

                    Label { text: "Baud"; color: "#b8c1cb"; font.pixelSize: 12 * root.uiScale }
                    TextField {
                        id: baudField
                        Layout.fillWidth: true
                        Layout.minimumWidth: 0
                        text: viewModel ? String(viewModel.baudRate) : "9600"
                        inputMethodHints: Qt.ImhDigitsOnly
                        font.pixelSize: 12 * root.uiScale
                    }

                    Label { text: "Poll (ms)"; color: "#b8c1cb"; font.pixelSize: 12 * root.uiScale }
                    TextField {
                        id: pollField
                        Layout.fillWidth: true
                        Layout.minimumWidth: 0
                        text: viewModel ? String(viewModel.pollingIntervalMs) : "1000"
                        inputMethodHints: Qt.ImhDigitsOnly
                        font.pixelSize: 12 * root.uiScale
                    }

                    Button {
                        Layout.fillWidth: true
                        text: viewModel && viewModel.connectionStateText !== "Idle" ? "Disconnect" : "Connect"
                        onClicked: {
                            if (!viewModel) {
                                return
                            }

                            viewModel.portName = portField.text

                            const baud = Number(baudField.text)
                            if (!Number.isNaN(baud) && baud > 0) {
                                viewModel.baudRate = baud
                            }

                            const poll = Number(pollField.text)
                            if (!Number.isNaN(poll) && poll > 0) {
                                viewModel.pollingIntervalMs = poll
                            }

                            if (viewModel.connectionStateText !== "Idle") {
                                viewModel.disconnectFromUart()
                            } else {
                                viewModel.connectToUart()
                            }
                        }
                    }

                    Button {
                        Layout.fillWidth: true
                        text: "Refresh Ports"
                        onClicked: {
                            if (viewModel) {
                                viewModel.reloadAvailablePorts()
                            }
                        }
                    }

                    Button {
                        Layout.fillWidth: true
                        text: "Read Now"
                        enabled: root.onlineLike
                        onClicked: {
                            if (viewModel) {
                                viewModel.refreshNow()
                            }
                        }
                    }

                    Label {
                        Layout.fillWidth: true
                        Layout.minimumWidth: 0
                        text: {
                            if (!viewModel) {
                                return "No backend"
                            }
                            if (viewModel.errorCode !== 0 && viewModel.errorMessage.length > 0) {
                                return "E" + viewModel.errorCode + ": " + viewModel.errorMessage
                            }
                            return viewModel.statusMessage
                        }
                        color: root.onlineLike ? "#40f067" : "#ef4444"
                        font.pixelSize: 12 * root.uiScale
                        elide: Text.ElideRight
                    }
                }
            }
        }
    }
}
