import QtQuick

Item {
    id: root

    property real padding: 12
    property real cornerRadius: 10
    default property alias contentData: contentItem.data
    clip: true

    Rectangle {
        anchors.fill: parent
        radius: root.cornerRadius
        color: "#2a2e33"
        border.color: "#6a737d"
        border.width: 1
    }

    Rectangle {
        width: parent.width * 0.28
        height: parent.height * 1.4
        rotation: 22
        x: parent.width * 0.3
        y: -parent.height * 0.2
        color: "#1cffffff"
    }

    Item {
        id: contentItem
        anchors.fill: parent
        anchors.margins: root.padding
    }
}
