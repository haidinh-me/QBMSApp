import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    id: root

    property var viewModel
    readonly property var snapshot: viewModel ? viewModel.snapshot : ({})
    readonly property var cells: snapshot.cells || []
    property int cellCount: cells.length > 0 ? cells.length : 8
    readonly property real packVoltage: Number(snapshot.packVoltage || 0)
    readonly property real packCurrent: Number(snapshot.current || 0)
    readonly property bool onlineLike: viewModel && (viewModel.connectionStateText === "Online" || viewModel.connectionStateText === "Degraded")

    readonly property real uiScale: parent.width/850
    readonly property real baseCellVoltage: {
        if (packVoltage <= 0) {
            return 3.64
        }
        return packVoltage / cellCount
    }

    readonly property real minCellVoltage: {
        if (cells.length === 0) {
            return baseCellVoltage - 0.02
        }
        let minValue = Number(cells[0])
        for (let i = 1; i < cells.length; i++) {
            const value = Number(cells[i])
            if (value < minValue) {
                minValue = value
            }
        }
        return minValue
    }

    readonly property real maxCellVoltage: {
        if (cells.length === 0) {
            return baseCellVoltage + 0.02
        }
        let maxValue = Number(cells[0])
        for (let i = 1; i < cells.length; i++) {
            const value = Number(cells[i])
            if (value > maxValue) {
                maxValue = value
            }
        }
        return maxValue
    }

    function cellVoltageFor(index) {
        if (cells.length > index) {
            return Number(cells[index])
        }
        const phase = (index % 4) - 1.5
        const wave = index % 2 === 0 ? 0.003 : -0.002
        return baseCellVoltage + phase * 0.004 + wave
    }

    AppCard {
        anchors.fill: parent
        radius: 8 * root.uiScale
        padding: 10 * root.uiScale

        ColumnLayout {
            anchors.fill: parent
            spacing: 8 * root.uiScale

            RowLayout {
                Layout.fillWidth: true

                Label {
                    text: "PACKS"
                    color: "#7ecaf2"
                    font.bold: true
                    font.pixelSize: 14 * root.uiScale
                }

                Item { Layout.fillWidth: true }

                Label {
                    text: viewModel ? ("UART " + viewModel.connectionStateText.toUpperCase()) : "UART IDLE"
                    color: viewModel && viewModel.connectionStateText === "Online" ? "#40f067" : "#ef4444"
                    font.bold: true
                    font.pixelSize: 12 * root.uiScale
                }
            }

            AppCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 66 * root.uiScale
                radius: 8 * root.uiScale
                backgroundColor: "#171b1f"

                RowLayout {
                    anchors.fill: parent
                    spacing: 14 * root.uiScale

                    InfoPair {
                        Layout.fillWidth: true
                        fontScale: root.uiScale
                        labelText: "Cells"
                        valueText: String(root.cellCount)
                    }

                    InfoPair {
                        Layout.fillWidth: true
                        fontScale: root.uiScale
                        labelText: "Điện áp"
                        valueText: Number(root.packVoltage).toFixed(2) + "V"
                    }

                    InfoPair {
                        Layout.fillWidth: true
                        fontScale: root.uiScale
                        labelText: "Dòng điện"
                        valueText: Number(root.packCurrent).toFixed(2) + "A"
                    }

                    Button {
                        text: "Refresh"
                        enabled: root.onlineLike
                        onClicked: {
                            if (viewModel) {
                                viewModel.refreshNow()
                            }
                        }
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
                    columns: Math.max(4, Math.min(8, Math.floor((width + columnSpacing) / (76 * root.uiScale))))
                    rowSpacing: 8 * root.uiScale
                    columnSpacing: 6 * root.uiScale

                    Repeater {
                        model: root.cellCount

                        BatteryCellBar {
                            Layout.alignment: Qt.AlignHCenter
                            Layout.preferredWidth: 70 * root.uiScale
                            Layout.preferredHeight: 110 * root.uiScale
                            cellIndex: index + 1
                            voltage: root.cellVoltageFor(index)
                            displayMinVoltage: 2.8
                            displayMaxVoltage: 3.65
                            isMinCell: root.cellVoltageFor(index) === root.minCellVoltage
                            isMaxCell: root.cellVoltageFor(index) === root.maxCellVoltage
                        }
                    }
                }
            }
        }
    }
}
