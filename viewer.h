#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QStringList>
#include <QKeyEvent>
#include <QTimer>
#include <QFileInfo>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QDir>

class ImageViewer : public QMainWindow {
    Q_OBJECT
public:
    explicit ImageViewer(QWidget *parent = nullptr) : QMainWindow(parent) {
        imageLabel = new QLabel(this);
        imageLabel->setAlignment(Qt::AlignCenter);
        setCentralWidget(imageLabel);
        setStyleSheet("background-color: black;");

        opacityEffect = new QGraphicsOpacityEffect(imageLabel);
        imageLabel->setGraphicsEffect(opacityEffect);
        fadeAnim = new QPropertyAnimation(opacityEffect, "opacity");
        fadeAnim->setDuration(800);

        overlayWidget = new QWidget(this);
        overlayWidget->setStyleSheet("background-color: rgba(0, 0, 0, 160); border-radius: 15px;");
        QHBoxLayout *layout = new QHBoxLayout(overlayWidget);

        QString btnStyle = "QPushButton { background: transparent; color: white; border: none; font-size: 22px; width: 45px; height: 45px; } "
        "QPushButton:hover { background: rgba(255, 255, 255, 40); border-radius: 10px; }";

        prevBtn = new QPushButton("❮", overlayWidget);
        slideBtn = new QPushButton("▶", overlayWidget);
        nextBtn = new QPushButton("❯", overlayWidget);
        prevBtn->setFocusPolicy(Qt::NoFocus); slideBtn->setFocusPolicy(Qt::NoFocus); nextBtn->setFocusPolicy(Qt::NoFocus);
        prevBtn->setStyleSheet(btnStyle); slideBtn->setStyleSheet(btnStyle); nextBtn->setStyleSheet(btnStyle);

        layout->addWidget(prevBtn);
        layout->addWidget(slideBtn);
        layout->addWidget(nextBtn);

        connect(prevBtn, &QPushButton::clicked, this, &ImageViewer::prevImage);
        connect(slideBtn, &QPushButton::clicked, this, &ImageViewer::toggleSlideshow);
        connect(nextBtn, &QPushButton::clicked, this, &ImageViewer::nextManual);

        slideshowTimer = new QTimer(this);
        connect(slideshowTimer, &QTimer::timeout, this, &ImageViewer::nextAuto);
        resize(1200, 800);
    }

    void addPaths(const QStringList &paths) {
        if (paths.isEmpty()) return;

        // Если прилетело больше 1 файла — это СТРОГОЕ ВЫДЕЛЕНИЕ
        if (paths.size() > 1) {
            imageList = paths;
            currentIndex = 0;
        } else {
            // Если прилетел 1 файл — это СКАН ПАПКИ
            QFileInfo fi(paths[0]);
            QDir dir = fi.dir();
            QStringList entries = dir.entryList({"*.jpg", "*.jpeg", "*.png", "*.webp", "*.jxl"}, QDir::Files, QDir::Name);
            imageList.clear();
            for (const QString &e : entries) imageList << dir.absoluteFilePath(e);
            currentIndex = qMax(0, imageList.indexOf(fi.absoluteFilePath()));
        }

        overlayWidget->setVisible(imageList.size() > 1);
        updateDisplay(false);
    }

protected:
    void resizeEvent(QResizeEvent *) override {
        overlayWidget->adjustSize();
        overlayWidget->move(width() - overlayWidget->width() - 25, 25);
        updateDisplay(false);
    }
    void keyPressEvent(QKeyEvent *e) override {
        if (e->key() == Qt::Key_Right) nextManual();
        else if (e->key() == Qt::Key_Left) prevImage();
        else if (e->key() == Qt::Key_Space) toggleSlideshow();
        else if (e->key() == Qt::Key_Escape) close();
    }

private slots:
    void nextManual() { if(imageList.isEmpty()) return; currentIndex = (currentIndex + 1) % imageList.size(); updateDisplay(false); }
    void nextAuto() { if(imageList.isEmpty()) return; currentIndex = (currentIndex + 1) % imageList.size(); updateDisplay(true); }
    void prevImage() { if(imageList.isEmpty()) return; currentIndex = (currentIndex - 1 + imageList.size()) % imageList.size(); updateDisplay(false); }
    void toggleSlideshow() {
        if (imageList.size() <= 1) return;
        isSlideshow = !isSlideshow;
        if (isSlideshow) { slideshowTimer->start(8000); slideBtn->setText("⏸"); }
        else { slideshowTimer->stop(); slideBtn->setText("▶"); }
    }

private:
    void updateDisplay(bool anim) {
        if (imageList.isEmpty() || currentIndex >= imageList.size()) return;
        QPixmap pix(imageList[currentIndex]);
        if (pix.isNull()) return;

        QFileInfo fi(imageList[currentIndex]);
        setWindowTitle(QString("%1 - %2x%3 - [%4 из %5] - Ultra HDR Viewer")
        .arg(fi.fileName()).arg(pix.width()).arg(pix.height())
        .arg(currentIndex + 1).arg(imageList.size()));

        imageLabel->setPixmap(pix.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        if (anim) {
            fadeAnim->stop(); fadeAnim->setStartValue(0.0); fadeAnim->setEndValue(1.0); fadeAnim->start();
        } else {
            fadeAnim->stop(); opacityEffect->setOpacity(1.0);
        }
    }

    QLabel *imageLabel;
    QWidget *overlayWidget;
    QPushButton *prevBtn, *slideBtn, *nextBtn;
    QStringList imageList;
    int currentIndex = 0;
    bool isSlideshow = false;
    QTimer *slideshowTimer;
    QGraphicsOpacityEffect *opacityEffect;
    QPropertyAnimation *fadeAnim;
};
#endif
