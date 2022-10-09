#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "STRINGS.h"
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
#include <QSpinBox>
#include <QObject>
#include <QTimer>
#include <QLabel>
#include <QComboBox>

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
    QLabel        * ID_Counter;
    QPushButton   * Prev_Button;
    QPushButton   * Play_Button;
    QPushButton   * Next_Button;

    QList<imgdata>     * imgList;
    QStandardItemModel * imgmodel;
    QStandardItemModel * changeframemodel;
    QStandardItemModel * animModel;
    Spriteflow         * previewPixmap;
    QTimer             * frametimer;

    int frametime;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool freshenUp();
    void populate_pointers();

    void setupScene();
    void setupHeaders();

    void openImage(QString);
    QPixmap getImage(QString);

private slots:
    void update_Pixmap();

    void on_last_frame_clicked();

    void on_start_clicked();

    void on_nextframe_clicked();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionExport_triggered();

    void on_actionSettings_triggered();

    void on_actionImport_Image_triggered();

    void on_Image_List_doubleClicked(const QModelIndex &index);

    void on_NewChangeframe_clicked();

    void on_NewAnimation_clicked();

    QList<QString> imgNames();//Sends images to select list

    void on_ID_slider_valueChanged(int value);

    void on_ID_slider_sliderPressed();

    void on_ID_slider_actionTriggered(int action);

    void stopPlayback();

    void on_ID_slider_sliderMoved(int position);

    void on_Anim_List_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
