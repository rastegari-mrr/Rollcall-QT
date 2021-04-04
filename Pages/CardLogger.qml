import QtQuick 2.12
import QtQuick.Controls 2.5

Item {
    id: root
    property string cardNo: ''
    signal cardRecieved(var cardNo)
    QtObject {
        id: props
        property string cardNo: ''
    }

    Repeater {
        model: 10
        delegate: Item {
            Action {
                enabled: root.enabled
                shortcut: Number(model.modelData).toString()
                onTriggered: {
                    props.cardNo = props.cardNo + Number(model.modelData).toString()
                }
            }
        }
    }
    Action {
        enabled: root.enabled
        shortcut: 'Return'
        onTriggered: {            
            var no = props.cardNo
            props.cardNo = ''
            if(no.length >= 10) {
                root.cardNo = no
                cardRecieved(no)
            }
        }
    }
}
