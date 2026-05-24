import QtQuick
import QtQuick.Controls

Item {
	id: root

	property int cellIndex: 1
	property real voltage: 3.2
	property real displayMinVoltage: 2.8
	property real displayMaxVoltage: 3.65
	property bool isMinCell: false
	property bool isMaxCell: false
	property real uiScaler: parent.width/900

	readonly property real level: {
		const range = displayMaxVoltage - displayMinVoltage
		if (range <= 0) {
			return 0
		}
		const ratio = (voltage - displayMinVoltage) / range
		return Math.max(0, Math.min(1, ratio))
	}

	readonly property color cellColor: {
		if (isMaxCell) {
			return "#0b6b2d"
		}
		if (isMinCell) {
			return "#f2d445"
		}
		return "#6fd9a3"
	}

	implicitWidth: 68
	implicitHeight: 102

	Column {
		anchors.fill: parent
		spacing: root.uiScaler*6

		Label {
			width: parent.width
			horizontalAlignment: Text.AlignHCenter
			text: String(root.cellIndex)
			color: "#bbc5cf"
			font.pixelSize: root.uiScaler*12
		}

		Item {
			width: parent.width
			height: parent.height/1.65

			Rectangle {
				id: terminal
				anchors.horizontalCenter: parent.horizontalCenter
				width: root.uiScaler*18
				height:root.uiScaler*5
				radius: root.uiScaler*2
				color: "#9aa2ad"
			}

			Rectangle {
				id: body
				anchors.top: terminal.bottom
				anchors.topMargin: 2
				anchors.horizontalCenter: parent.horizontalCenter
				width: root.uiScaler*40
				height: root.uiScaler*65
				radius: root.uiScaler*6
				color: "#121519"
				border.color: "#96a0ab"
				border.width: root.uiScaler*2

				Rectangle {
					anchors.left: parent.left
					anchors.right: parent.right
					anchors.bottom: parent.bottom
					anchors.margins: 4
					height: (parent.height - 8) * root.level
					radius: root.uiScaler*3
					color: root.cellColor
				}
			}
		}

		Label {
			width: parent.width
			horizontalAlignment: Text.AlignHCenter
			text: Number(root.voltage).toFixed(3) + "V"
			color: root.cellColor
			font.pixelSize: root.uiScaler*14
			font.bold: true
		}
	}
}
