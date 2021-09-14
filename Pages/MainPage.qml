import QtQuick 2.12
import QtQuick.Controls 2.5
import 'date-convert.js' as Convert
import 'number-format.js' as FormatNumbers
import QtGraphicalEffects 1.0
import Omega 1.0
import "../users.js" as Users

Page {
    id: root
    readonly property var restFunctions: {"none" : 0, "getAllUsers": 1, "addCardRead": 2, "addToListAndGetAllUsers": 3}
    property string dateStr
    QtObject {
        id: props
        property date date
        property int restApiType
        property bool isLoggerActive: false        
        property bool gotoUsers: false
    }

    function sendChequeNotify() {
        var hour = props.date.getHours()
        if(hour > 8) {
            chequeNotify.startGet();
            console.log('Cheque notify is sent')
        }
    }

    Component.onCompleted: {
        props.restApiType = restFunctions.getAllUsers
        console.log('main page entered')
        restApi.startGet("GetAllUsers", "")
        busy.visible = true
        var ret = userOperate.setDateTime(Users.dateStrToDate(root.dateStr))
        console.log('main page entered - ' + ret)
        props.date = new Date()
        sendChequeNotify()
    }

    StackView.onActivated: {
        props.isLoggerActive = true        
    }

    StackView.onDeactivated: {
        props.isLoggerActive = false
    }

    Timer {
        running: true
        repeat: true
        interval: 1000
        onTriggered: props.date = new Date()
    }
    Column {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top        
        anchors.topMargin: 70
        Text {
            id: lblTime
            color: lblDate.color
            anchors.horizontalCenter: parent.horizontalCenter
            text: FormatNumbers.convertToPersian(Convert.formatPersianDate(props.date, 'HH:mm:ss'))
            font.pixelSize: 90
            font.family: stackView.font.family            
            font.bold: true
        }
        Text {
            color: lblDate.color
            text: FormatNumbers.convertToPersian(Convert.formatPersianDate(props.date, 'dddd'))
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 50
            font.family: stackView.font.family
        }
        Text {
            id: lblDate
            color: "#191fa6"
            text: FormatNumbers.convertToPersian(Convert.formatPersianDate(props.date, 'dd MMMM yyyy'))
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 50
            font.family: stackView.font.family
        }
    }

    Image {
        id: imgCard
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 100
        source: "/Icons/card.svg"
        width: 250
        height: 250
        fillMode: Image.PreserveAspectFit
        visible: true
        MouseArea {
            anchors.fill: parent
            onClicked: {
                props.gotoUsers = true
                busy.visible = true
                props.restApiType = restFunctions.addToListAndGetAllUsers
                restApi.startGet("AddToListAndGetAllUsers", Users.makeAddToListArgs())
            }
        }
    }

    UserLog {
        id: showUser
        anchors.fill: parent
        userOperate: userOperate
        onFinished: {
            Users.addCardRecieveds();
        }
    }

    CardLogger {
        id: logger
        enabled: props.isLoggerActive && !busy.visible && !showUser.visible
        onCardRecieved: {
            console.log(cardNo)
            Users.cardRecieved(cardNo)
        }
    }

    BusyIndicator {
        id: busy
        width: 400
        anchors.centerIn: parent
        visible: false
    }

    ProgressFileUpload {
        id: progressBar
        anchors.fill: parent
        visible: true
        //onVisibleChanged: console.log('progress bar visible : ' + visible)
    }

    Message {
        id: messageDialog
        anchors.fill: parent
    }

    Timer {
        id: syncTimer
        interval: 120000
        repeat: true
        onTriggered: {
            Users.addCardRecieveds();            
            sendChequeNotify()
        }
    }

    ChequeNotify {
        id: chequeNotify
    }

    RestAPI {
        id: restApi
        onReplied: {
            busy.visible = false
            var type = props.restApiType
            props.restApiType = restFunctions.none
            switch(type) {
            case restFunctions.getAllUsers:
                Users.getAllUsers(json)
                //logger.enabled = true
                syncTimer.start();
                break;
            case restFunctions.addCardRead:
                var j = JSON.parse(json)
                if(j.success === 'true') {
                    Users._cardRecieveds = []
                    Users.saveCardsToFile()
                } else
                    messageDialog.show(j.message)
                break;
            case restFunctions.addToListAndGetAllUsers:
                var res = JSON.parse(json);
                if(res.success === 'true') {
                    Users.getAllUsers(json, true)
                }
                break;
            default:
                console.log(json)
                break;
            }
        }
        onTimeout: {
            if(messange === '') {
                var type = props.restApiType
                props.restApiType = restFunctions.none
                busy.visible = false
                messageDialog.show('No answer recieved')
            } else
                messageDialog.show(messange)
        }
    }


    UserOperate {
        id: userOperate
         onDownloadsFinished: {
             progressBar.visible = false
             if(props.gotoUsers) {
                 props.gotoUsers = false
                 stackView.push("/Pages/Users.qml", {users: Users._users})
             }
         }
         onTimeout: {
             progressBar.visible = false
             //console.log('user operate - timeout')
         }

         onProgressChanged: {
             //console.log(currentFile + ' - ' + percent + '% , ' + totalFiles)
             progressBar.value = percent
             progressBar.currentInex = currentFile
             progressBar.totalFiles = totalFiles
         }
    }
}





/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
