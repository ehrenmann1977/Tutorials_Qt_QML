import QtQuick 2.12
import QtQuick.Controls 2.5
import com.drOmranConsulting.backend 1.0


ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Tabs")

    BackEnd {
        id: _backend
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Page1Form {
            objectName: "page1box"
            /* changecoordinate
               this is a signal from QML to C++
               bound in main.cpp with the backend object
            */
            signal changecoordinate(msgx: double,  msgy: double)

            /* qmlReceive
              this is a signal in QML that is bounded to another signal
              readQml on the cpp side. Goal of this signal is
              Send signal from cpp to signal in Qml
              transfer data from cpp to qml
             */
            signal qmlReceive(msgr: double)

            /* updateR
               is called internally onXChanged and onYChanged
               functions, this will update the x,y coordinates
               in the C++ side using signal changecoordinate
            */
            function updateR() {
                console.log("updateR position changed to : x="+_rect.x + " y=>"+_rect.y)
                //to control the object inside the ui use this example
                //_lbl.text = qsTr("x=" + rectid.x + " y=" + rectid.y)

                //emit signal that is bounded to C++ code
                console.log ("updateR: emitting changecoordinate signal")
                changecoordinate (_rect.x, _rect.y)

                //console.log("string "+ _aString)

                console.log("updateR: request to read rec_x from cpp")
                console.log("updateR: received value =>" + _backend.getRecX())

            }

            /* myQmlFunction
               this is an example how to call a qml function
               from c++ and read its return
             */
            function myQmlFunction(msg: variant) : variant {
                console.log("Got message:", msg)
                return "some return value"
            }

            _rect.onXChanged: updateR()
            _rect.onYChanged: updateR()


            onQmlReceive:
            {
                console.log("signal received from cpp -> new x position is " + msgr)
                _rect.x=msgr
            }

            /* initialize coordinates in C++ side */
            Component.onCompleted:
            {
                updateR()
            }

        }


        Page2Form {
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("Test Page")
        }
        TabButton {
            text: qsTr("About Page")
        }
    }
}
