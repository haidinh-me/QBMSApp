/// @brief BMSApp - Qt6 QML application for BMS monitoring and control

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "src/viewmodels/bmsviewmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    BmsViewModel bmsViewModel;
    QQmlApplicationEngine engine;
    
    engine.rootContext()->setContextProperty("bmsViewModel", &bmsViewModel);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    
    engine.loadFromModule("BMSApp", "Main");

    return app.exec();
}
