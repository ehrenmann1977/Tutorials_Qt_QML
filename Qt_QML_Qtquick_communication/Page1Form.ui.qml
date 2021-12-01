import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    width: 600
    height: 400
    property alias _rect: rectid
    property alias _lbl: lbl

    header: Label {
        text: qsTr("Drag Box Demo")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    Item {
        id: maindrawing
        width: parent.width
        height: parent.height

        Label {
            id:lbl
            text: qsTr("x=" + rectid.x + " y=" + rectid.y)
            anchors.top: rectid.bottom
            anchors.left: rectid.left
        }


        Rectangle {
            id: rectid
            x: 85
            y: 46
            width: 112
            height: 66
            color: "red"

            MouseArea {
                id: mousearea
                anchors.fill: parent
                drag.target: parent
            }
        }

    }


}
