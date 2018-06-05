#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include "imageprocessor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QImage _image;
    QImage _current;

    ImageProcessor _imgproc;

    void setImage(const QImage& img);

    void openFile();
    void saveFile();
    void reset();

    void setImageBackground(QColor color);
};

#endif // MAINWINDOW_H
