#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <spriteflow.h>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTableView>
#include <QSlider>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

QGraphicsView * Pre_View;
QTableView    * Changeframe_Table;
QTableView    * Image_Table;
QTableView    * Anim_Table;
QSlider       * ID_Slider;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void populate_pointers();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
