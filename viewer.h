#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QStringList>
#include <QPropertyAnimation>

class ImageViewer : public QMainWindow {
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);
    void setImageList(const QStringList &list, int index);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private slots:
    void nextImage();
    void prevImage();
    void toggleSlideshow();
    void updateImage();

private:
    QLabel *imageLabel;
    QStringList imageList;
    int currentIndex = 0;
    double zoomFactor = 1.0;
    bool isSlideshowActive = false;
    QTimer *slideshowTimer;
    void showImage(int index);
};

#endif
