import QtQuick

Item {
    id: root

    property real radius: 8
    property real padding: 10
    property color backgroundColor: "#1f2428"
    property color borderColor: "#525b65"

    default property alias contentData: contentHost.data

    Rectangle {
        anchors.fill: parent
        radius: root.radius
        color: root.backgroundColor
        border.color: root.borderColor
        border.width: 1
    }

    Item {
        id: contentHost
        anchors.fill: parent
        anchors.margins: root.padding
    }
}
