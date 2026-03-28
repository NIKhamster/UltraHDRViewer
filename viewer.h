#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QLabel>
#include <vector>
#include <QString>

class UltraViewer : public QMainWindow {
    Q_OBJECT
public:
    explicit UltraViewer(const std::vector<QString>& files, QWidget *parent = nullptr);
protected:
    void keyPressEvent(class QKeyEvent *event) override;
    void wheelEvent(class QWheelEvent *event) override;
    void resizeEvent(class QResizeEvent *event) override;
private:
    void nextImage();
    void prevImage();
    void updateDisplay();

    std::vector<QString> fileList;
    int currentIndex;
    double zoomFactor;
    QLabel *imageLabel;
    QLabel *infoOverlay;
    class QPushButton *btnLeft;
    class QPushButton *btnRight;
};

#endif
