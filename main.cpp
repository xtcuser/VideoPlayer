#include "videoplayer.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QtCore/QDir>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QScreen>

//#include <QCursor>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;


    app.setStyle("Fusion");

    QCommandLineParser parser;
    parser.setApplicationDescription("Onur's video player");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("url", "The URL to open.");
    parser.process(app);

    VideoPlayer player;
    if (!parser.positionalArguments().isEmpty()) {
    }
    player.resize(800,480);

////    For the project that will make videos play in fullscreen <3

//    player.resize(1920,1080);
//    QRect screenrect = app.primaryScreen()->geometry();
//    player.move(screenrect.left(), screenrect.top());
//    player.setWindowFlags(Qt::CustomizeWindowHint);

////    For the project that will make videos play in fullscreen <3

    player.show();
    player.setWindowTitle("Video Player v1.01 by Closx - ");

////    To hide mousecursor on QT application += include <QCursor>

//    QCursor cursor(Qt::BlankCursor);
//    QApplication::setOverrideCursor(cursor);
//    QApplication::changeOverrideCursor(cursor);

////    To hide mousecursor on QT application += include <QCursor>

    return app.exec();
}
