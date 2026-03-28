#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QVariantAnimation>
#include <QGraphicsOpacityEffect>
#include <QFileInfo>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPixmap>
#include <QPushButton>
#include <QIcon>
#include <QImageReader>
#include <QColorSpace>
#include <QSurfaceFormat>
#include <QWidget>
#include <vector>

class UltraViewer : public QMainWindow {
public:
    UltraViewer(const std::vector<QString>& files) : fileList(files), currentIndex(0), zoomFactor(1.0) {
        // Настройка HDR
        QSurfaceFormat format;
        format.setColorSpace(QColorSpace::SRgb);
        QSurfaceFormat::setDefaultFormat(format);

        // Установка иконки окна
        setWindowIcon(QIcon("icon.png"));

        setStyleSheet("background-color: #000;");

        central = new QWidget(this);
        setCentralWidget(central);
        imageLabel = new QLabel(central);
        imageLabel->setAlignment(Qt::AlignCenter);

        opacityEffect = new QGraphicsOpacityEffect(this);
        imageLabel->setGraphicsEffect(opacityEffect);

        fadeAnim = new QVariantAnimation(this);
        fadeAnim->setStartValue(0.0);
        fadeAnim->setEndValue(1.0);
        connect(fadeAnim, &QVariantAnimation::valueChanged, this, [this](const QVariant &v){
            opacityEffect->setOpacity(v.toDouble());
        });

        controls = new QWidget(this);
        // БЕЗ обводки
        controls->setStyleSheet("background: rgba(20, 20, 20, 230); border-radius: 12px; border: none;");

        QString s = "QPushButton { background: transparent; color: white; font-size: 24px; border: none; } "
        "QPushButton:hover { background: rgba(255,255,255,60); border-radius: 8px; }";

        btnPrev = new QPushButton("<", controls);
        btnPlay = new QPushButton("▶", controls);
        btnNext = new QPushButton(">", controls);

        btnPrev->setFocusPolicy(Qt::NoFocus);
        btnPlay->setFocusPolicy(Qt::NoFocus);
        btnNext->setFocusPolicy(Qt::NoFocus);

        btnPrev->setStyleSheet(s); btnPlay->setStyleSheet(s); btnNext->setStyleSheet(s);

        slideTimer = new QTimer(this);
        connect(slideTimer, &QTimer::timeout, this, [this](){ changeImage(1, 650); });

        hideTimer = new QTimer(this);
        connect(hideTimer, &QTimer::timeout, this, [this](){ controls->hide(); });

        connect(btnPrev, &QPushButton::clicked, this, [this](){ changeImage(-1, 250); });
        connect(btnNext, &QPushButton::clicked, this, [this](){ changeImage(1, 250); });
        connect(btnPlay, &QPushButton::clicked, this, [this](){ toggleSlideshow(); });

        setMouseTracking(true);
        central->setMouseTracking(true);
        imageLabel->setMouseTracking(true);

        if (!fileList.empty()) updateDisplay();
        else controls->hide();
    }

protected:
    void keyPressEvent(QKeyEvent *e) override {
        if (e->key() == Qt::Key_Right) changeImage(1, 250);
        else if (e->key() == Qt::Key_Left) changeImage(-1, 250);
        // Пробел теперь запускает слайд-шоу, как кнопка Play
        else if (e->key() == Qt::Key_Space) toggleSlideshow();
        else if (e->key() == Qt::Key_Escape) close();
    }

    void mouseMoveEvent(QMouseEvent *) override {
        if(fileList.size() > 1) {
            controls->show();
            hideTimer->start(2500);
        }
    }

    void resizeEvent(QResizeEvent *) override {
        imageLabel->setGeometry(0, 0, width(), height());
        controls->setGeometry(width() - 160, 25, 140, 60);
        btnPrev->setGeometry(5, 5, 40, 50);
        // Play приподнята на 3 пикселя вверх (Y=2)
        btnPlay->setGeometry(50, 2, 40, 50);
        btnNext->setGeometry(95, 5, 40, 50);
    }

    void wheelEvent(QWheelEvent *e) override {
        if (e->angleDelta().y() > 0) { if (zoomFactor < 10.0) zoomFactor *= 1.1; }
        else { if (zoomFactor > 0.1) zoomFactor *= 0.9; }
        updateDisplay();
    }

private:
    void toggleSlideshow() {
        if(slideTimer->isActive()) {
            slideTimer->stop();
            btnPlay->setText("▶");
        } else {
            // Сначала ждем, потом скипаем
            slideTimer->start(4500);
            btnPlay->setText("||");
        }
    }

    void changeImage(int step, int duration) {
        if(fileList.size() < 2) return;
        currentIndex = (currentIndex + step + (int)fileList.size()) % (int)fileList.size();
        zoomFactor = 1.0;
        fadeAnim->stop();
        fadeAnim->setDuration(duration);
        updateDisplay();
        fadeAnim->start();
    }

    void updateDisplay() {
        if (fileList.empty()) return;
        QImageReader reader(fileList[currentIndex]);
        reader.setAutoTransform(true);
        QImage img = reader.read();
        if (img.isNull()) return;

        bool isHDR = (img.depth() > 32);
        Qt::TransformationMode mode = (zoomFactor > 1.1) ? Qt::FastTransformation : Qt::SmoothTransformation;

        imageLabel->setPixmap(QPixmap::fromImage(img).scaled(size() * zoomFactor, Qt::KeepAspectRatio, mode));

        setWindowTitle(QString("%1 UltraViewer — %2 (%3/%4)")
        .arg(isHDR ? "[HDR]" : "")
        .arg(QFileInfo(fileList[currentIndex]).fileName())
        .arg(currentIndex + 1).arg(fileList.size()));
    }

    std::vector<QString> fileList;
    int currentIndex;
    double zoomFactor;
    QLabel *imageLabel;
    QWidget *central;
    QWidget *controls;
    QPushButton *btnPrev, *btnNext, *btnPlay;
    QTimer *slideTimer, *hideTimer;
    QGraphicsOpacityEffect *opacityEffect;
    QVariantAnimation *fadeAnim;
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    std::vector<QString> f;
    for (int i = 1; i < argc; ++i) f.push_back(QString::fromLocal8Bit(argv[i]));
    UltraViewer w(f);
    w.showMaximized();
    return a.exec();
}
