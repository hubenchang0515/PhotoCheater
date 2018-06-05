#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    connect(ui->action_open, QAction::triggered, this, MainWindow::openFile);
    connect(ui->action_save, QAction::triggered, this, MainWindow::saveFile);

    connect(ui->action_exit, QAction::triggered, QApplication::quit);
    connect(ui->action_reset, QAction::triggered, this, MainWindow::reset);
//    connect(ui->action_gray, QAction::triggered, this, MainWindow::gray);
//    connect(ui->action_binary, QAction::triggered, this, MainWindow::binary);
//    connect(ui->action_average, QAction::triggered, this, MainWindow::average);
//    connect(ui->action_median, QAction::triggered, this, MainWindow::median);
//    connect(ui->action_gaussian, QAction::triggered, this, MainWindow::gaussian);

    connect(ui->action_white, QAction::triggered, [this](){setImageBackground(0xffffff);});
    connect(ui->action_red, QAction::triggered, [this](){setImageBackground(0xff0000);});
    connect(ui->action_green, QAction::triggered, [this](){setImageBackground(0x00ff00);});
    connect(ui->action_blue, QAction::triggered, [this](){setImageBackground(0x0000ff);});
    connect(ui->action_yellow, QAction::triggered, [this](){setImageBackground(0xffff00);});
    connect(ui->action_purple, QAction::triggered, [this](){setImageBackground(0xff00ff);});
    connect(ui->action_cyan, QAction::triggered, [this](){setImageBackground(0x00ffff);});
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setImage(const QImage& img)
{
    _current = img;
    ui->display->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::openFile()
{
    QString name = QFileDialog::getOpenFileName(this);
    _image.load(name);
    setImage(_image);
}


void MainWindow::saveFile()
{
    QString name = QFileDialog::getSaveFileName(this);
    _current.save(name);
}

void MainWindow::reset()
{
    setImage(_image);
}

void MainWindow::setImageBackground(QColor color)
{
    setImage(_imgproc.changeBackground(_image, color));
}
