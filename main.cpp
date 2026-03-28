#include <QApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <QFileInfo>
#include <QIcon>
#include <QTimer>
#include "viewer.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon("/usr/local/share/icons/ultrahdrviewer.png"));

    QString serverName = "UltraHDR_Final_Instance_Fixed";
    QLocalSocket socket;
    socket.connectToServer(serverName);

    // Если копия уже есть — шлем файл и выходим
    if (socket.waitForConnected(200)) {
        if (argc > 1) {
            QStringList toSend;
            for (int i = 1; i < argc; ++i)
                toSend << QFileInfo(QString::fromLocal8Bit(argv[i])).absoluteFilePath();
            socket.write(toSend.join("|").toUtf8());
            socket.waitForBytesWritten();
        }
        return 0;
    }

    QLocalServer server;
    server.listen(serverName);

    ImageViewer viewer;
    QStringList pendingPaths;

    // Таймер сборки: ждем, пока Dolphin докидает все выделенные файлы
    QTimer *processTimer = new QTimer(&app);
    processTimer->setSingleShot(true);
    processTimer->setInterval(150); // 150мс хватит, чтобы собрать "пачку"

    QObject::connect(processTimer, &QTimer::timeout, [&]() {
        viewer.addPaths(pendingPaths);
        pendingPaths.clear();
        viewer.show();
    });

    QObject::connect(&server, &QLocalServer::newConnection, [&]() {
        QLocalSocket *client = server.nextPendingConnection();
        QObject::connect(client, &QLocalSocket::readyRead, [&, client, processTimer]() {
            QString data = QString::fromUtf8(client->readAll());
            pendingPaths << data.split("|", Qt::SkipEmptyParts);
            processTimer->start(); // Сбрасываем таймер при каждом новом файле
        });
    });

    if (argc > 1) {
        for (int i = 1; i < argc; ++i)
            pendingPaths << QFileInfo(QString::fromLocal8Bit(argv[i])).absoluteFilePath();
        processTimer->start();
    } else {
        viewer.show();
    }

    return app.exec();
}
