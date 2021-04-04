import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import Omega 1.0
import QtQuick.Controls.Material 2.2

Page {
    id: root
    property var users
    header: ToolBar {
        background: Rectangle {
            color: Material.color(Material.BlueGrey)
            implicitHeight: 80
        }
        GridLayout {
                    anchors.fill: parent
                    rows: 1
                    columns: 2
                    ToolButton {
                        id: btnBack
                        Layout.preferredHeight: 80
                        Layout.alignment: Qt.AlignCenter
                        Layout.leftMargin: 20
                        icon.source: '/Icons/back.svg'
                        onClicked: stackView.pop()
                        Layout.fillHeight: true
                        Layout.topMargin: 0
                        Layout.bottomMargin: -10
                        padding: 0
                    }

                    Text {
                        text: 'کاربران'
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignCenter
                        Layout.leftMargin: 20
                        Layout.rightMargin: 20
                        horizontalAlignment: Qt.RightToLeft
                        font.pointSize: 20
                        color: 'white'
                        font.family: stackView.font.family
                    }
                }
    }

    ListView {
        id: listView
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        model: root.users
        delegate: GridLayout {
            columns: 2
            rows: 2
            anchors.left: parent.left
            anchors.right: parent.right
            layoutDirection: Qt.RightToLeft
            Rectangle {
                Layout.preferredWidth: 100
                Layout.preferredHeight: 100
                Layout.maximumWidth: 100
                Layout.maximumHeight: 100
                Layout.minimumWidth: 100
                Layout.minimumHeight: 100
                Layout.rowSpan: 2
                border.width: 1
                border.color: 'black'
                Image {
                    height: parent.height
                    fillMode: Image.PreserveAspectFit
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: fileName(modelData.id)
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        busy.title = 'در انتظار کارت'
                        busy.visible = true
                        logger.id = modelData.id
                        logger.index = index
                        logger.enabled = true
                    }
                }
            }

            Text {
                text: modelData.name
                font.family: stackView.font.family
                font.bold: true
                font.pixelSize: 30
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.row: 0
                Layout.column: 1
                Layout.topMargin: 10
            }
            Text {
                text: 'شماره کارت : ' + modelData.CardNo
                font.family: stackView.font.family
                Layout.row: 1
                Layout.column: 1
                Layout.bottomMargin: 10
            }

        }
    }

    Rectangle {
        id: busy
        property string title
        anchors.fill: parent
        color: "#AA000000"
        BusyIndicator {
            id: busyIndicator
            width: 400
            anchors.centerIn: parent
        }
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: busyIndicator.bottom
            text: busy.title
            font.family: stackView.font.family
            color: 'white'
        }
        visible: false
    }

    CardLogger {
        id: logger
        property string id
        property int index
        enabled: false
        onCardRecieved: {
            busy.title = 'در حال ارسال'
            restApi.startGet('SaveCardNo', 'id=' + id + '&cardNo=' + cardNo)
        }
    }

    Message {
        id: messageDialog
        anchors.fill: parent
    }


    UserOperate {
        id: userOperate
    }

    RestAPI {
        id: restApi

        onReplied: {
            busy.visible = false
            var j = JSON.parse(json);
            if(j.success !== 'true') {
                console.log(j.message)
            } else {
                console.log(logger.index + ' - ' + logger.cardNo)
                root.users[logger.index].CardNo = logger.cardNo
                listView.model = null
                listView.model = root.users
                listView.positionViewAtIndex(logger.index, ListView.Beginning)
            }
        }

        onTimeout: {
            busy.visible = false
            if(messange === '') {
                messageDialog.show('No answer recieved')
            }
            messageDialog.show(messange)

        }
    }

    function fileName(id) {
        var f = '/home/pi/' + id + '.jpg'
        if(userOperate.fileExists(f))
            return 'image://myprovider/extern?path=' + f
        return '/Icons/person.svg'
    }
}
