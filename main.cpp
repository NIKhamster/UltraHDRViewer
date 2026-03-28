#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QSurfaceFormat>
#include <QColorSpace>
#include "viewer.h"

int main(int argc, char *argv[]) {
    // Настройка HDR (должна быть до создания QApplication)
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setColorSpace(QColorSpace::SRgbLinear);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);

    ImageViewer viewer;

    if (argc > 1) {
        QString firstFile = QString::fromLocal8Bit(argv[1]);
        QFileInfo fileInfo(firstFile);

        if (fileInfo.isFile()) {
            QDir dir = fileInfo.dir();
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

            if (!fullPaths.isEmpty()) {
                viewer.setImageList(fullPaths, startIndex);
            }
        }
    }

    viewer.show();
    return a.exec();
}
