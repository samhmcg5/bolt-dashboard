#Code Documentation for BOLT MkIII Touchscreen  Dashboard
###Written in Qt 


##OVERVIEW:
    The code for the dashboard is split into several source and header files.
    
    "main.cpp" creates an instance of the window, defines how large to create it (Maximized, Normal, or Fullscreen), and starts the event loop.

    "mainwindow.cpp" and "mainwindow.h" contain the code for the overall look and behavior of the GUI. The constructor for class 'MainWindow' handles the instantiation of gauges, threads, layout managers, and slots. After the GUI is created, the MainWindow constructor spins off a thread to manage CAN. 

    "qgauge.cpp" and "qgauge.h" contain classes needed for the custom gauge widgets used to display RPM and temperatures. These are open source code found on the internet but have been slightly adapted for BOLT's needs.

    "interfacecan.cpp" and "interfacecan.h" handle fetching messages from the CAN bus and then updating the GUI based on the messages. These rely on code found in the "can" directory of this project.


##INTERFACECAN:
    This file manages communication with the CAN bus with the class "InterfaceCan" which is a subclass of QThread. When an instance of "InterfaceCan" is constructed, it sits idly until its "run()" method is called. Calling the "run()" method starts the execution of the thread. 

    When executing, the thread iterated through an infinite while loop, receiving data from the CAN bus with the "canreceive()" function. This returns a struct called "canfd_frame", defined in "can.h". 

    The message received over the bus is parsed based on its ID and certain variables representing the condition of the bike are updated. When a variable is updated, a QT signal is emited containing its new value. This is how the GUI sees the most recent data from the bus and updates its widgets. 

    There is a separate signal for each variable present. 


QGAUGE
    This is a custom widget created by a third party. Unfortunately, it is poorly documented and not written in a very transparent manner. 

    The gauges that are seen on the GUI are actually comprised of several smaller widgets. Primarily, a needle, a color ring, a label, a value label, and a ring of numbers. 

    The gauges are not very customizable within the same project since customization is done within the source code rather than configuration functions. This being said, it may be a good idea to create a custom widget specifically for the purposes of BOLT. 


##MAINWINDOW:

    === 
    ###TODO:
    Save Preference parameters to a file, load them on startup of the GUI, send over CAN bus and save on change.
    ===

    The majority of code in this class deals with configuring various widgets for the GUI. 

    The layout and design of the GUI is managed mostly with QT Designer and the "mainwindow.ui" file. Graphically creating the interface made designing the code much simpler and cleaner. 

    The main widget of the GUI is a QStackedWidget which is organized into pages. Page 1 is the driver view, holding the gauges and battery state widgets. Page 2 is the Debug view, showing the most recent value of all CAN messages received from the InterfaceCan class. Page 3 is the rider prefereces page, which will send messages back to the CAN bus to edit the VMC settings. Navigation between pages is handled with signals from buttons on the screen and slots that are defined in the MainWindow class.

    The signals emited from the InterfaceCan class are connected to a set of slots in the MainWindow class. These slots update LCD numbers on the debug page as well as update gauges and the battery indicator. 

    In the constructor, the line "ui->setupUi(this)" instantiates the UI defined by QT designer. The "initGauge*()" functions create instances of QGauge classes, which are placed into the GUI in the "initLayout()" function.

    The "initLayout()" function creates a new QVBoxLayout manager, adds it to an existing layout manager created from QT Designer, then adds temperature gauges to it.
