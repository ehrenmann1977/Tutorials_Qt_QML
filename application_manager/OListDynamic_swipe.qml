import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Item {

    property string addnewitem:""
    property int removeitemindex: -1
    property string appenditemstring: ""
    property int appenditemindx:-1
    property int fontpoint: 20
    property int radiuspoint: 14
    property int spacingvalue: 0
    property string delegate_color:"beige"
    property string delegate_border_color:"yellowgreen"
    property string highlight_color:"deeppink"
    signal selectedvalueSignal (string iTemstring)
    property string sv: ""
    property int indexcopy:0
    id:lstmodelitem
    width: parent.width
    height: parent.height

    ListModel {
                id: mListModel
                ListElement { sender: "Bob Bobbleton"; title: "How are you going?" }
                ListElement { sender: "Rug Emporium"; title: "SALE! All rugs MUST go!" }
                ListElement { sender: "Electric Co."; title: "Electricity bill 15/07/2016 overdue" }
                ListElement { sender: "Tips"; title: "Five ways this tip will save your life" }
            }


    ListView {
        id: mListViewId
        anchors.fill: parent
        model: mListModel


        Layout.fillWidth : true
        Layout.fillHeight: true
        clip: true
        snapMode: ListView.SnapToItem //this stops the view at the boundary
        spacing: spacingvalue
        focus: true

//        highlight: Rectangle
//        {
//            id: highlightid
//            width: parent.width
//            color: highlight_color
//            border.color: "beige"
//            z:3
//            opacity: 0.2

//        }
//        highlightRangeMode: ListView.StrictlyEnforceRange
//        highlightFollowsCurrentItem:true
//        onCurrentIndexChanged:
//        {
//            console.log("olistdynamic_swipe Indexchanged" + currentIndex)
//            mListViewId.currentIndex=currentIndex
//            lstmodelitem.selectedvalueSignal(currentIndex)
//            indexcopy=currentIndex

//        }

        delegate:
            SwipeDelegate {
            id: swipeDelegate
            text: model.firstName //+ " - " + model.title
            width: parent.width
            spacing:10




            ListView.onRemove: SequentialAnimation {
                PropertyAction {
                    target: swipeDelegate
                    property: "ListView.delayRemove"
                    value: true
                }
                NumberAnimation {
                    target: swipeDelegate
                    property: "height"
                    to: 0
                    easing.type: Easing.InOutQuad
                }
                PropertyAction {
                    target: swipeDelegate
                    property: "ListView.delayRemove"
                    value: false
                }
            }


//            swipe.right: Button {
//                id: deleteLabel
//                text: qsTr("Delete")
//                //color: "white"
//                //verticalAlignment: Label.AlignVCenter
//                padding: 12
//                height: parent.height
//                anchors.right: parent.right

//                onClicked: {
//                    console.log("button remove clicked")
//                    mListModel.remove(index)
//                }

//                SwipeDelegate.onClicked: {
//                    console.log("clicked remove")
//                    mListModel.remove(index)
//                }

//                background: Rectangle {
//                    color: deleteLabel.SwipeDelegate.pressed ? Qt.darker("tomato", 1.1) : "tomato"
//                }
//            }

            swipe.right: Rectangle {
                width: parent.width
                height: parent.height

                clip: true
                color: SwipeDelegate.pressed ? "#555" : "#666"

                Label {
                    font.family: "Fontello"
                    text: swipeDelegate.swipe.complete ? "\ue805" // icon-cw-circled
                                                  : "\ue801" // icon-cancel-circled-1

                    padding: 20
                    anchors.fill: parent
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter

                    opacity: 2 * -swipeDelegate.swipe.position

                    //color: Material.color(delegate.swipe.complete ? Material.Green : Material.Red, Material.Shade200)
                    Behavior on color { ColorAnimation { } }
                }

                Label {
                    text: qsTr("Removed")
                    color: "white"

                    padding: 20
                    anchors.fill: parent
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter

                    opacity: swipeDelegate.swipe.complete ? 1 : 0
                    Behavior on opacity { NumberAnimation { } }
                }

                SwipeDelegate.onClicked: swipeDelegate.swipe.close()
                SwipeDelegate.onPressedChanged: undoTimer.stop()
            }
            //! [delegate]

            //! [removal]
            Timer {
                id: undoTimer
                interval: 15
                onTriggered:mListModel.remove(index);
            }

            swipe.onCompleted: undoTimer.start()
            //! [removal]




            Component.onCompleted: {
                swipeDelegate.clicked.connect(catchclick)
            }

            function catchclick(){
                lstmodelitem.selectedvalueSignal(modelData)
                mListViewId.currentIndex=index
                indexcopy=index
                console.log("selected item =" + model.firstName + " current index " + model.index)

            }

        }

        //! [transitions]
        remove: Transition {
            SequentialAnimation {
                PauseAnimation { duration: 125 }
                NumberAnimation { property: "height"; to: 0; easing.type: Easing.InOutQuad }
            }
        }

        displaced: Transition {
            SequentialAnimation {
                PauseAnimation { duration: 125 }
                NumberAnimation { property: "y"; easing.type: Easing.InOutQuad }
            }
        }
        //! [transitions]

        ScrollIndicator.vertical: ScrollIndicator { }


    }

    function getindex()
    {
        console.log("current index = " + indexcopy)
        return mListViewId.currentIndex
    }

    function setindex(index)
    {
        //console.log("olistdynamic set index"+index)
        mListViewId.currentIndex=index
    }

    function add2Item(newEntry){
        mListModel.append({"firstName": newEntry})
    }


    function deleteItem(index){
        mListModel.remove(index,1)
    }

    function appendIdem(index,valueEntry)
    {
        mListModel.set(index,{"firstName": valueEntry})
    }


    onAddnewitemChanged: {
        console.log("added item" + addnewitem)
        add2Item(addnewitem)
    }
    //remove item with index
    onRemoveitemindexChanged: {
        console.log("remove item")
        deleteItem(removeitemindex)
    }

    //to change the item, change the index first then the string
    onAppenditemstringChanged: {
        appendIdem(appenditemindx,appenditemstring)
    }


}
