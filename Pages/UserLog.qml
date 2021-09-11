import QtQuick 2.12
import QtQuick.Controls 2.5
import Omega 1.0

Page {
    id: root
    property var user
    property var userOperate
    property bool isAcceptedBefore: true    
    opacity: 0
    visible: false
    signal finished

    QtObject {
        id: props
        property string fileName: ''
    }


    function show(user, isAcceptedBefore) {
        root.user = user
        root.isAcceptedBefore = isAcceptedBefore
        props.fileName = fileName()
        animation.start()        
    }

    function fileName() {
        var f = '/home/pi/' + user.id + '.jpg'
        if(userOperate.fileExists(f))
            return 'image://myprovider/extern?path=' + f
        return '/Icons/person.svg'
    }


    SequentialAnimation {
        id: animation
        onFinished: root.finished()
        PropertyAction { target: root; property: 'visible'; value: 'true'}
        NumberAnimation { target: root; property: 'opacity'; to: 1.0; duration: 500}
        NumberAnimation { duration: 5000 }
        NumberAnimation { target: root; property: 'opacity'; to: 0; duration: 500}
        PropertyAction { target: root; property: 'visible'; value: 'false'}
    }

    Column {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        spacing: 5
        Image {
            source: props.fileName
            height: 300
            fillMode: Image.PreserveAspectFit
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Text {
            id: lblDate
            color: "#191fa6"
            text: root.user !== undefined ? root.user.name : ''
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 35
            font.bold: true
            font.family: stackView.font.family
        }
        Text {
            text: ' '
        }
        Text {
            color: "#191fa6"
            text: root.isAcceptedBefore ? 'شما قبلا تایید شده اید' : 'شما تایید شدید'
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 35
            font.bold: true
            font.family: stackView.font.family
        }
    }
}
