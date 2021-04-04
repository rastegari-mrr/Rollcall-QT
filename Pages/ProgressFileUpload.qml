import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.2

Rectangle {
    id: progressBar
    property int value: 0
    property int currentInex: 0
    property int totalFiles: 0
    anchors.fill: parent
    color: "#AA000000"
    Column {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.leftMargin: 50
        //spacing: 5
        ProgressBar {
            from: 0
            to: 100
            height: 20
            value: progressBar.value
            Material.accent: Material.Blue
        }
        Text {
            id: lblPercent
            anchors.horizontalCenter: parent.horizontalCenter
            text: progressBar.value + '% - ' + progressBar.currentInex + ' of ' + progressBar.totalFiles
            color: 'white'
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
