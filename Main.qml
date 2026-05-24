import QtQuick
import QtQuick.Controls
import "qml/components"

Window {
    width: 1000
    height: 600
    visible: true
    color: "#0f1115"
    title: qsTr("BMS Application")

    //flags: Qt.FramelessWindowHint
    //visibility: Window.FullScreen

    AppShell {
        anchors.fill: parent
        viewModel: bmsViewModel
    }
}
