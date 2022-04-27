import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    id: page
    width: 600
    height: 400

    Label {
        id: header
        height: 37
        text: qsTr("Info")
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        font.pixelSize: 16
        wrapMode: Text.Wrap
        textFormat: Text.RichText
        bottomPadding: 10
        topPadding: 10
        padding: 15
    }

    Flickable{
        id: flickable
        ScrollBar.vertical: ScrollBar{}
        clip: true
        width: parent.width
        height: parent.height-header.height-lower_strip.height
        anchors.top: header.bottom;anchors.topMargin: 20
        anchors.bottom: lower_strip.top; anchors.bottomMargin: 20
        contentWidth: parent.width
        contentHeight: parent.height -header.height + 1

        boundsBehavior: Flickable.OvershootBounds
        Row{
            id: row
            height: flickable.height
            width: flickable.width

            Column {
                id: column1
                anchors.left: parent.left
                anchors.right: column2.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.rightMargin: 10
                anchors.leftMargin: 10
                anchors.topMargin: 10
                anchors.bottomMargin: 10
                width: parent.width- column2.width


                Label {
                    text: qsTr("Application Manager is a program written by Dr. Sherif Omran from www.embedded-ib.de for following cases: a- freelancers are trying to apply for projects, and they need to manage to which project they applied for, especially at the end of the year whey they prepare the tax document, each application will be equal to 2.5 Euros. b- small companies that do have many applications for a certain job and they need to manage the applicants, notes and attachments for later use")
                    anchors.fill: parent
                    font.letterSpacing: 0.2
                    wrapMode: Text.WordWrap
                    font.family: "Verdana"
                    font.hintingPreference: Font.PreferDefaultHinting
                    font.wordSpacing: 1
                    renderType: Text.NativeRendering
                    textFormat: Text.PlainText
                }
            }
            Column {
                id: column2
                width: image.width
                height: parent.height
                anchors.right: parent.right; anchors.rightMargin: 10
                anchors.top: parent.top;
                anchors.bottom: parent.bottom;


                Image {
                    id: image
                    width: 100
                    height: 100
                    source: "qrc:/qtquickplugin/images/template_image.png"
                    fillMode: Image.PreserveAspectFit
                }
            }


        }

    }

    Row {
        id: lower_strip
        height: 30
        width: parent.width
        anchors.top: row.bottom
        anchors.bottom: parent.bottom; anchors.bottomMargin: 20
        CheckBox {
            id: checkBox
            anchors.left: parent.left; anchors.leftMargin: 10
            text: qsTr("Save Setting")
        }

        ToolSeparator {
            id: toolSeparator
            width: parent.width-checkBox.width-button.width - 20
            opacity: 0
            visible: true
        }

        Button {
            id: button
            anchors.right: parent.right
            anchors.rightMargin: 30
            text: qsTr("SKIP")
        }

    }


}
