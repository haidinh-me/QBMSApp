import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property string title: ""
    property string valueText: "--"
    property color valueColor: "#4ef07f"
    property real scaleFactor: 1.0
    property int valuePixelSize: 26

    implicitWidth: 150 * scaleFactor
    implicitHeight: 76 * scaleFactor
    clip: true

    Rectangle {
        anchors.fill: parent
        radius: 8 * scaleFactor
        color: "#1e2227"
        border.color: "#4f5761"
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8 * scaleFactor
        spacing: 2 * scaleFactor

        Label {
            text: root.title
            color: "#b4bbc4"
            font.pixelSize: 12 * scaleFactor
            elide: Text.ElideRight
            Layout.fillWidth: true
            Layout.minimumWidth: 0
        }

        Label {
            text: root.valueText
            color: root.valueColor
            font.bold: true
            font.pixelSize: root.valuePixelSize * scaleFactor
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            Layout.fillWidth: true
            Layout.minimumWidth: 0
        }
    }
}
