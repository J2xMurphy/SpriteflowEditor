#include "mainwindow.h"
#include "./ui_mainwindow.h"

QGraphicsScene * EZScene;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    populate_pointers();
    EZScene = new QGraphicsScene;
    Pre_View->setScene(EZScene);
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

void MainWindow::on_last_frame_clicked()
{

}


void MainWindow::on_start_clicked()
{

}


void MainWindow::on_nextframe_clicked()
{

}


void MainWindow::on_ID_slider_sliderMoved(int position)
{

}


void MainWindow::on_Image_List_cellDoubleClicked(int row, int column)
{

}


void MainWindow::on_Change_List_cellDoubleClicked(int row, int column)
{

}


void MainWindow::on_Anim_List_cellDoubleClicked(int row, int column)
{

}


void MainWindow::on_actionNew_triggered()
{

}


void MainWindow::on_actionOpen_triggered()
{

}


void MainWindow::on_actionSave_triggered()
{

}


void MainWindow::on_actionSave_As_triggered()
{

}


void MainWindow::on_actionExport_triggered()
{

}


void MainWindow::on_actionSettings_triggered()
{

}


void MainWindow::on_actionImport_Image_triggered()
{
    QString dir;
    dir = QFileDialog::getOpenFileName(this,
        "Open...", "", "(*.jpg);;(*.png);;(*.jpeg)");
    openImage(dir);
}

void MainWindow::openImage(QString dir)
{
    QPixmap qxp((QString(dir)));
    QStandardItem * whee = new QStandardItem(1,2);

    Image_Table->set
    EZScene->addPixmap(qxp);
}
