import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property var tabs: ["GROUP", "PACKS", "SETTING"]
    property int currentIndex: 0
    property real fontScale: 1.0
    signal tabSelected(int index)

    RowLayout {
        anchors.fill: parent
        spacing: 6 * root.fontScale

        Repeater {
            model: root.tabs.length

            delegate: Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Rectangle {
                    anchors.fill: parent
                    radius: 6 * root.fontScale
                    color: root.currentIndex === index ? "#f0f0f0" : "#d0d3d8"
                    border.color: root.currentIndex === index ? "#ffffff" : "#9ba0a8"
                }

                Rectangle {
                    width: parent.width
                    height: 3
                    anchors.top: parent.top
                    color: root.currentIndex === index ? "#38d46a" : "transparent"
                }

                Label {
                    anchors.centerIn: parent
                    text: root.tabs[index]
                    color: "#23272c"
                    font.bold: true
                    font.pixelSize: 13 * root.fontScale
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: root.tabSelected(index)
                }
            }
        }
    }
}
