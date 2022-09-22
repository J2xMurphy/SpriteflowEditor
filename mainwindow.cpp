#include "mainwindow.h"
#include "./ui_mainwindow.h"

QGraphicsScene * EZScene;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    populate_pointers();
    setupScene();
    imgList = new QList<imgdata>;
    imgmodel = new QStandardItemModel();
    Image_Table->setModel(imgmodel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupScene()
{
    EZScene = new QGraphicsScene;
    Pre_View->setScene(EZScene);
    previewPixmap = new Spriteflow();
    setScene(EZScene);
    previewPixmap->sendToScene();
}

void MainWindow::populate_pointers(){
Pre_View = findChild<QGraphicsView*>("Preview_View");
Changeframe_Table = findChild<QTableWidget*>("Change_List");
Image_Table = findChild<QTableView*>("Image_List");
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
        "Open...", "", "(*.jpg *.png jpeg)");
    openImage(dir);
}

void MainWindow::openImage(QString dir)
{
    QPixmap qxp((QString(dir)));
    QString imgname = dir.sliced(dir.lastIndexOf("/")+1);
    imgmodel->appendRow(new QStandardItem(imgname));

    imgdata newImg;
    newImg.name = imgname;
    newImg.img = qxp;
    imgList->append(newImg);

    previewPixmap->play(qxp);

}

void MainWindow::on_Image_List_doubleClicked(const QModelIndex &index)
{
    QString target = index.data().toString();
    previewPixmap->play(getImage(target));
}

QPixmap MainWindow::getImage(QString target)
{
    for(imgdata id : *imgList){
        if (target == id.name)
            return id.img;
    }
    QPixmap fail;
    return fail;
}

void MainWindow::on_pushButton_clicked()
{
    QDialog cf_create(this);
    QHBoxLayout cf_layout(&cf_create);
    cf_create.setWindowTitle("New Changeframe");

    auto goto_title = new QLabel("Goto:");
    cf_layout.addWidget(goto_title);
    QSpinBox * gotobox = new QSpinBox();
    cf_layout.addWidget(gotobox);

    auto space1 = new QSpacerItem(2,2);
    cf_layout.addItem(space1);

    auto label_title = new QLabel("Label:");
    cf_layout.addWidget(label_title);
    QSpinBox * labelbox = new QSpinBox();
    cf_layout.addWidget(labelbox);

    auto accept_button = new QPushButton("Accept");
    cf_layout.addWidget(accept_button);
    QObject::connect(accept_button,SIGNAL(clicked()),&cf_create,SLOT(accept()));

    cf_create.exec();
    previewPixmap->addChangeframe(gotobox->value(),labelbox->value());

    delete goto_title;
    delete gotobox;
//  delete space1; Results in a SEGFAULT for some reason
    delete label_title;
    delete labelbox;
    delete accept_button;

}
