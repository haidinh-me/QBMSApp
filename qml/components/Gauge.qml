import QtQuick
import QtQuick.Controls

Item {
	id: root

	property real from: 0
	property real to: 100
	property real value: 0
	property real scaleFactor: 1.0
	property color trackColor: "#394049"
	property color fillColor: "#42ee6d"
	property color textColor: "#f3f7f4"
	property int strokeWidth: Math.max(6, Math.round(12 * scaleFactor))

	readonly property real ratio: {
		const range = to - from
		if (range <= 0) {
			return 0
		}
		const r = (value - from) / range
		return Math.max(0, Math.min(1, r))
	}

	implicitWidth: 140 * scaleFactor
	implicitHeight: 140 * scaleFactor

	Canvas {
		id: gaugeCanvas
		anchors.fill: parent

		onPaint: {
			const ctx = getContext("2d")
			const w = width
			const h = height
			const centerX = w / 2
			const centerY = h / 2
			const radius = Math.min(w, h) / 2 - root.strokeWidth

			ctx.reset()
			ctx.lineCap = "round"

			ctx.beginPath()
			ctx.lineWidth = root.strokeWidth
			ctx.strokeStyle = root.trackColor
			ctx.arc(centerX, centerY, radius, 0, Math.PI * 2)
			ctx.stroke()

			ctx.beginPath()
			ctx.lineWidth = root.strokeWidth
			ctx.strokeStyle = root.fillColor
			ctx.arc(
				centerX,
				centerY,
				radius,
				-Math.PI / 2,
				-Math.PI / 2 + Math.PI * 2 * root.ratio,
				false
			)
			ctx.stroke()
		}
	}

	Label {
		anchors.centerIn: parent
		text: Math.round(root.value) + "%"
		color: root.textColor
		font.bold: true
		font.pixelSize: 26 * scaleFactor
	}

	onRatioChanged: gaugeCanvas.requestPaint()
	onWidthChanged: gaugeCanvas.requestPaint()
	onHeightChanged: gaugeCanvas.requestPaint()
}
