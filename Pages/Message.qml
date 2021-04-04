import QtQuick 2.12
import QtQuick.Controls 2.5
import Omega 1.0

Rectangle {
    id: root
    color: "#AA000000"
    property string message
    visible: false
    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        height: parent.height / 2
        Text {
            id: lblMessage
            text: root.message
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            font.family: stackView.font.family
            wrapMode: Text.WordWrap
        }
    }

    function show(message) {
        root.message = message
        animation.start()
    }

    SequentialAnimation {
        id: animation
        PropertyAction { target: root; property: 'visible'; value: 'true'}
        NumberAnimation { target: root; property: 'opacity'; to: 1.0; duration: 500}
        NumberAnimation { duration: 5000 }
        NumberAnimation { target: root; property: 'opacity'; to: 0; duration: 500}
        PropertyAction { target: root; property: 'visible'; value: 'false'}
    }
}
