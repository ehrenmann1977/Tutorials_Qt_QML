# Qt_QML_QtQuick_communication
This project illustrates the following how to

## connect UI Component to QtQuick for separating the ui from the logic
here is an example. 

 In Page1Form.ui.qml and to expose the Rectangle item
      Rectangle {
      id: rectid
      }

within the Page item

      property alias _rect: rectid
      property alias _lbl: lbl
    
main.qml
to read the item values use this

     _rect.x
   
to fetch any UI changes use this

     _rect.onXChanged: updateR()
     _rect.onYChanged: updateR()

## connect QtQuick item's signal to C++ function to update the c++ backend with a new x-coordinate
here is an example

#main.qml

    Page1Form 
    {
    objectNmae: "page1box"
    signal changecoordinate(msgx: double,  msgy: double)
    }

#main.cpp

    QQmlApplicationEngine engine;

    //Backend class Initialization
    backend Backend;
    qmlRegisterType<backend>("com.drOmranConsulting.backend",1,0,"BackEnd");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
           return -1;


    //Qml -> C++ connection
    // connect rectangle change coordinate signal coming from qml to backend dataChange_from_qml slot
    QObject *rootObject = engine.rootObjects().first();
    QObject *pageOne = rootObject->findChild<QObject*>("page1box");
    QObject::connect(pageOne, SIGNAL(changecoordinate(double,double)),&Backend,SLOT(dataChange_from_qml(double,double)));



## connect C++ signal to QtQuick signal (signal-signal) so that we can read data from Qt Quick functions from C++ logic backend
here is an example
main.cpp


    //C++ -> Qml SIGNAL SIGNAL Connection
    QObject::connect(&Backend,SIGNAL(informQmlXChange(double)),pageOne,SIGNAL(qmlReceive(double)));

backend.cpp

    QVariant returnedValue;
    QVariant msg = "Hello from C++";
    QMetaObject::invokeMethod(p1, "myQmlFunction", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, msg));
    qDebug() << "QML function returned:" << returnedValue;
    
main.qml

     /* qmlReceive
     this is a signal in QML that is bounded to another signal
     readQml on the cpp side. Goal of this signal is
     Send signal from cpp to signal in Qml
     transfer data from cpp to qml
     */
     signal qmlReceive(msgr: double)
            
     /* myQmlFunction
     this is an example how to call a qml function
     from c++ and read its return
     */
     function myQmlFunction(msg: variant) : variant {
     console.log("Got message:", msg)
     return "some return value"
     }
            


## all QML objects memory pointers are encapsulated into a QMAP from the main.cpp so that we can read any function from QML from C++ subclasses
here is an example
main.cpp

    //Use local thread object to store pageOne pointer in order to access local qml functions
    //from backend class
    memRefPtr memobj;
    memobj.ltsRegisterObject("page1box",pageOne);

backend.cpp
    memRefPtr memobjekt;
    QObject* p1=memobjekt.ltsGetObject("page1box");

----------------------------------
## License

No commercial license is granted, for private use and educational purposes.


Written by
Dr. Sherif Omran
----------------------------------
Dr. Omran Consulting Ingenieurbüro
Germany
