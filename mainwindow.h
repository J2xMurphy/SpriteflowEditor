#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTableView>
#include <QSlider>
#include <spriteflow.h>
#include <QFile>
#include <iostream>
#include <QFileDialog>
#include <QDebug>
#include <QPixmap>
#include <QStandardItemModel>
#include <QStandardItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct imgdata{
    QString name;
    QPixmap img;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QGraphicsView * Pre_View;
    QTableView    * Changeframe_Table;
    QTableView    * Image_Table;
    QTableView    * Anim_Table;
    QSlider       * ID_Slider;

    QList<imgdata>     * imgList;
    QStandardItemModel * imgmodel;
    Spriteflow         * previewPixmap;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void populate_pointers();

    void setupScene();

    void openImage(QString);

    QPixmap getImage(QString);

private slots:
    void on_last_frame_clicked();

    void on_start_clicked();

    void on_nextframe_clicked();

    void on_ID_slider_sliderMoved(int position);

    void on_Change_List_cellDoubleClicked(int row, int column);

    void on_Anim_List_cellDoubleClicked(int row, int column);

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionExport_triggered();

    void on_actionSettings_triggered();

    void on_actionImport_Image_triggered();

    void on_Image_List_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
