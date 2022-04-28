import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    id: page
    width: 600
    height: 400

    header:
        Label {
        id:pagelable
        text: qsTr("Page 2")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    Row {
        id: row
        y: 0
        height: 56
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.top: pagelable.bottom

        Column {
            id: column
            width: 200
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.topMargin: 0
            anchors.bottomMargin: 0

            Image {
                id: image
                anchors.fill: parent
                source: "add-icon.png"
                fillMode: Image.PreserveAspectFit
            }
        }

        Column {
            id: column1
            width: 200
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.topMargin: 0

            Image {
                id: image1
                anchors.fill: parent
                source: "info-icon.jpg"
                fillMode: Image.PreserveAspectFit
            }
        }

        Column {
            id: column2
            width: 200
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.topMargin: 0

            Image {
                id: image2
                anchors.fill: parent
                source: "qrc:/qtquickplugin/images/template_image.png"
                fillMode: Image.PreserveAspectFit
            }
        }


    }

    Row {
        id: row1
        y: 62
        height: 287
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: row.bottom
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        OListDynamic_swipe {
            id: dynlist
            anchors.fill: parent
        }

    }
}
