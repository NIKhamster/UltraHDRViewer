// main.cpp - Updated with Folder Scanning and Smart Navigation
#include <QApplication>
#include <QImageReader>
#include <QDir>
#include <QFileInfo>
#include "viewer.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Включаем поддержку HDR
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setColorSpace(QColorSpace::SRgbLinear);
    QSurfaceFormat::setDefaultFormat(format);

    ImageViewer viewer;

    if (argc > 1) {
        QString firstFile = QString::fromLocal8Bit(argv[1]);
        QFileInfo fileInfo(firstFile);
        QDir dir = fileInfo.dir();

        // Сканируем папку на наличие картинок
        QStringList filters;
        filters << "*.jpg" << "*.jpeg" << "*.png" << "*.webp" << "*.jxl";
        QStringList images = dir.entryList(filters, QDir::Files, QDir::Name);

        QStringList fullPaths;
        int startIndex = 0;
        for (int i = 0; i < images.size(); ++i) {
            QString path = dir.absoluteFilePath(images[i]);
            fullPaths.append(path);
            if (path == fileInfo.absoluteFilePath()) {
                startIndex = i;
            }
        }

        viewer.setImageList(fullPaths, startIndex);
    }

    viewer.show();
    return a.exec();
}
