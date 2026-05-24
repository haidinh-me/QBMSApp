import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    id: root

    property var viewModel
    readonly property var snapshot: viewModel ? viewModel.snapshot : ({})
    readonly property real packVoltage: Number(snapshot.packVoltage || 0)
    readonly property real packCurrent: Number(snapshot.current || 0)
    readonly property int packSoc: Number(snapshot.soc || 0)
    readonly property real packTempC: Number(snapshot.temperatureC || 0)
    readonly property real remCapacityAh: Number(snapshot.remainingCapacityAh || 0)
    readonly property real designCapacityAh: Number(snapshot.designCapacityAh || 0)
    readonly property int cycleLife: Number(snapshot.cycleLife || 0)
    readonly property var cells: snapshot.cells || []
    readonly property var alarmList: snapshot.alarms || []
    readonly property bool onlineLike: viewModel && (viewModel.connectionStateText === "Online" || viewModel.connectionStateText === "Degraded")

    readonly property real uiScale: parent.width/850

    readonly property real maxCellVoltage: {
        if (cells.length > 0) {
            let maxV = Number(cells[0])
            for (let i = 1; i < cells.length; ++i) {
                const value = Number(cells[i])
                if (value > maxV) {
                    maxV = value
                }
            }
            return maxV
        }
        return (packVoltage > 0 ? packVoltage / 16.0 : 3.64) + 0.04
    }
    readonly property real minCellVoltage: {
        if (cells.length > 0) {
            let minV = Number(cells[0])
            for (let i = 1; i < cells.length; ++i) {
                const value = Number(cells[i])
                if (value < minV) {
                    minV = value
                }
            }
            return minV
        }
        return (packVoltage > 0 ? packVoltage / 16.0 : 3.64) - 0.04
    }
    readonly property real packPowerKw: Math.abs(packVoltage * packCurrent) / 1000.0

    AppCard {
        anchors.fill: parent
        radius: 8 * root.uiScale
        padding: 10 * root.uiScale

        Loader {
            anchors.fill: parent
            sourceComponent: wideLayout
        }
    }

    Component {
        id: wideLayout

        Item {
            RowLayout {
                anchors.fill: parent
                spacing: 10 * root.uiScale

                AppCard {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 255 * root.uiScale
                    Layout.minimumWidth: 210 * root.uiScale
                    radius: 8 * root.uiScale

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 8 * root.uiScale

                        Label {
                            text: "GROUP"
                            color: "#7ecaf2"
                            font.bold: true
                            font.pixelSize: 14 * root.uiScale
                        }

                        Item {
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            Gauge {
                                anchors.centerIn: parent
                                scaleFactor: root.uiScale
                                from: 0
                                to: 100
                                value: root.packSoc
                                fillColor: "#40f067"
                            }
                        }
                    }
                }

                AppCard {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: 8 * root.uiScale

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 8 * root.uiScale

                        GridLayout {
                            Layout.fillWidth: true
                            columns: 2
                            rowSpacing: 8 * root.uiScale
                            columnSpacing: 8 * root.uiScale

                            ValueTile {
                                Layout.fillWidth: true
                                scaleFactor: root.uiScale
                                title: "Điện áp"
                                valueText: Number(root.packVoltage).toFixed(2) + "V"
                            }

                            ValueTile {
                                Layout.fillWidth: true
                                scaleFactor: root.uiScale
                                title: "Dòng điện"
                                valueText: {
                                    if (!viewModel) {
                                        return "--.--A"
                                    }
                                    const sign = root.packCurrent > 0 ? "+" : ""
                                    return sign + Number(root.packCurrent).toFixed(2) + "A"
                                }
                            }

                            ValueTile {
                                Layout.fillWidth: true
                                scaleFactor: root.uiScale
                                title: "Dung lượng thiết kế (AH)"
                                valueText: Number(root.designCapacityAh).toFixed(1)
                                valuePixelSize: 22
                            }

                            ValueTile {
                                Layout.fillWidth: true
                                scaleFactor: root.uiScale
                                title: "Dung lượng còn lại (AH)"
                                valueText: (Number(root.packSoc)/100 * Number(root.designCapacityAh)).toFixed(1)
                                valuePixelSize: 22
                            }
                        }

                        AppCard {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            radius: 8 * root.uiScale
                            backgroundColor: "#171b1f"

                            GridLayout {
                                anchors.fill: parent
                                columns: 3
                                rowSpacing: 4 * root.uiScale
                                columnSpacing: 10 * root.uiScale

                                InfoPair {
                                    Layout.fillWidth: true
                                    fontScale: root.uiScale
                                    labelText: "Max.Cell"
                                    valueText: Number(root.maxCellVoltage).toFixed(3) + "V"
                                }
                                InfoPair {
                                    Layout.fillWidth: true
                                    fontScale: root.uiScale
                                    labelText: "Nhiệt độ"
                                    valueText: Number(root.packTempC).toFixed(1) + "℃"
                                }
                                InfoPair {
                                    Layout.fillWidth: true
                                    fontScale: root.uiScale
                                    labelText: "Sạc"
                                    valueText: root.packCurrent > 0 ? "ON" : "OFF"
                                    valueColor: root.packCurrent > 0 ? "#40f067" : "#9aa3ad"
                                }

                                InfoPair {
                                    Layout.fillWidth: true
                                    fontScale: root.uiScale
                                    labelText: "Min.Cell"
                                    valueColor: "#f2d445"
                                    valueText: Number(root.minCellVoltage).toFixed(3) + "V"
                                }
                                InfoPair {
                                    Layout.fillWidth: true
                                    fontScale: root.uiScale
                                    labelText: "Công xuất xả"
                                    valueText: Number(root.packPowerKw).toFixed(2) + "kW"
                                }
                                InfoPair {
                                    Layout.fillWidth: true
                                    fontScale: root.uiScale
                                    labelText: "Xả"
                                    valueText: root.packCurrent < 0 ? "ON" : "OFF"
                                    valueColor: root.packCurrent < 0 ? "#40f067" : "#9aa3ad"
                                }

                                InfoPair {
                                    Layout.fillWidth: true
                                    fontScale: root.uiScale
                                    labelText: "Chu kỳ sạc"
                                    valueText: Number(root.cycleLife)
                                }
                                InfoPair {
                                    Layout.fillWidth: true
                                    fontScale: root.uiScale
                                    labelText: "Alarm"
                                    valueText: root.alarmList.length > 0 ? root.alarmList[0] : "None"
                                    valueColor: root.alarmList.length > 0 ? "#f97316" : "#40f067"
                                }
                                InfoPair {
                                    Layout.fillWidth: true
                                    fontScale: root.uiScale
                                    labelText: "Trạng thái"
                                    valueText: viewModel ? viewModel.connectionStateText : "Idle"
                                    valueColor: root.onlineLike ? "#40f067" : "#ef4444"
                                }
                            }
                        }

                        AppCard {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 56 * root.uiScale
                            radius: 8 * root.uiScale
                            backgroundColor: "#171b1f"

                            RowLayout {
                                anchors.fill: parent
                                spacing: 8 * root.uiScale

                                Label {
                                    Layout.fillWidth: true
                                    Layout.minimumWidth: 0
                                    text: viewModel ? viewModel.statusMessage : "No backend"
                                    color: "#c3c9d1"
                                    font.pixelSize: 12 * root.uiScale
                                    elide: Text.ElideRight
                                }

                                Button {
                                    text: "Read"
                                    enabled: root.onlineLike
                                    onClicked: {
                                        if (viewModel) {
                                            viewModel.refreshNow()
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
