import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property string labelText: ""
    property string valueText: ""
    property color labelColor: "#b7bec7"
    property color valueColor: "#43f06e"
    property real fontScale: 1.0

    implicitHeight: 20 * fontScale

    RowLayout {
        anchors.fill: parent
        spacing: 6 * root.fontScale

        Label {
            text: root.labelText
            color: root.labelColor
            font.pixelSize: 12 * root.fontScale
        }

        Label {
            Layout.fillWidth: true
            Layout.minimumWidth: 0
            text: root.valueText
            color: root.valueColor
            font.bold: true
            font.pixelSize: 12 * root.fontScale
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignRight
        }
    }
}
