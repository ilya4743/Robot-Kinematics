#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QScreen>
#include <QStyleHints>
#include <QWindow>
#include "viewmodel/kinematics.h"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    Kinematics kinematics;
    QGuiApplication::styleHints()->setColorScheme(Qt::ColorScheme::Light);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("kinematics", &kinematics);
    engine.loadFromModule("KinematicsApp", "Main");

    auto* window = qobject_cast<QWindow*>(engine.rootObjects().value(0));
    if (!window) {
        return -1;
    }

    window->create();
    const QRect workArea = window->screen()->availableGeometry();
    const QSize frameSize = window->frameGeometry().size();
    window->setFramePosition(workArea.topLeft() + QPoint(
        (workArea.width() - frameSize.width()) / 2,
        (workArea.height() - frameSize.height()) / 2));
    window->show();

    return app.exec();
}
