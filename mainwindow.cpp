#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    populate_pointers();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::populate_pointers(){
Pre_View = findChild<QGraphicsView*>("Preview_View");
Changeframe_Table = findChild<QTableWidget*>("Change_List");
Image_Table = findChild<QTableWidget*>("Image_List");
Anim_Table = findChild<QTableWidget*>("Anim_List");
ID_Slider = findChild<QSlider*>("ID_slider");
}
