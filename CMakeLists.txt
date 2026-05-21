## @brief BMSApp - Battery Management System Monitoring Application

cmake_minimum_required(VERSION 3.16)

project(BMSApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick SerialPort)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appBMSApp
    main.cpp
)

qt_add_qml_module(appBMSApp
    URI BMSApp
    QML_FILES
        Main.qml
        SOURCES
        SOURCES src/backend/bmsserialreader.h src/backend/bmsserialreader.cpp
        SOURCES src/backend/bmsmanager.h src/backend/bmsmanager.cpp
        SOURCES src/backend/bmssnapshot.h
        SOURCES src/backend/jbdprotocol.h src/backend/jbdprotocol.cpp
        SOURCES src/backend/gpioFanController.h src/backend/gpioFanController.cpp
        SOURCES src/viewmodels/bmsviewmodel.cpp
        SOURCES src/viewmodels/bmsviewmodel.h

        QML_FILES qml/components/AppCard.qml
        QML_FILES qml/components/AppShell.qml
        QML_FILES qml/components/BatteryCellBar.qml
        QML_FILES qml/components/BottomTabBar.qml
        QML_FILES qml/components/InfoPair.qml
        QML_FILES qml/views/DashboardView.qml
        QML_FILES qml/views/PacksView.qml
        QML_FILES qml/views/SettingsView.qml
        QML_FILES qml/components/Gauge.qml
        QML_FILES qml/components/ValueTile.qml

)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appBMSApp PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appBMSApp
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appBMSApp
    PRIVATE Qt6::Quick
    PRIVATE Qt6::SerialPort
)

include(GNUInstallDirs)
install(TARGETS appBMSApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
