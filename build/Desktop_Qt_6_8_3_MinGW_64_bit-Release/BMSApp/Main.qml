import QtQuick
import QtQuick.Controls
import BMSApp

Window {
    width: 1000
    height: 600
    visible: true
    title: qsTr("BMS Application")

    Text{
        anchors{
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }
        text: "Hello"
        font.pixelSize: parent.width/4
    }
}
