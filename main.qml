import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.VirtualKeyboard 2.2
import QtQuick.Controls.Material 2.2
import Omega 1.0
ApplicationWindow {
    id: window
    visible: true
    width: 320
    height: 480
    Material.accent: Material.Blue

    property int tries: 0

    Component.onCompleted: {
        testRestApi();
    }

//    StackView {
//        id: stackView
//        anchors.fill: parent
//        font.family: 'Segoe UI'
//        initialItem: 'Pages/MainPage.qml'
//    }    
    function testRestApi() {
        lblIP.text = 'IP : ' + userOperate.getIP()
        restApi.startGet('', '')
    }

    RestAPI {
        id: restApi
        onReplied: {
            console.log(json)
            var j = JSON.parse(json)
            if(j.message === 'success') {
                loading.visible = false
                delete restApi;
                console.log(j.now)
                stackView.push('Pages/MainPage.qml', {dateStr: j.now});
            }
            else {
                lblLog.text = json
                testRestApi()
            }
        }
        onTimeout: {
            if(messange === '') {
                tries++;
                lblLog.text = 'timeout - tries : ' + tries
                testRestApi()
            }
            else
                lblLog.text = messange
        }

        onError: {
            lblLog.text = messange
            timer.start()
        }
    }

    Timer {
        id: timer
        interval: 2000
        onTriggered: {
            lblLog.text = 'waiting...'
            testRestApi()
        }
    }

    UserOperate {
        id: userOperate
    }

    Rectangle {
        id: loading
        color: '#00000000'
        clip: true
        width: 320
        height: 480
        Rectangle {
            anchors.fill: parent
            Column {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                BusyIndicator {
                    width: 400
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Text {
                    text: 'Loading'
                    font.family: stackView.font.family
                    font.pixelSize: 30
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
        Text {
            id: lblIP
            anchors.bottom: lblLog.top
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Text {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            id: lblLog
            text: '....'
        }

    }

    StackView {
        id: stackView
        anchors.fill: parent
        font.family: 'Segoe UI'
    }

    InputPanel {
        id: inputPanel
        z: 99
        x: 0
        y: window.height
        width: window.width

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: window.height - inputPanel.height
            }
        }
        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }
}
