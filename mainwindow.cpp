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
    changeframemodel = new QStandardItemModel();
    animModel = new QStandardItemModel();

    Image_Table->setModel(imgmodel);
    Changeframe_Table->setModel(changeframemodel);
    Anim_Table->setModel(animModel);

    changeframemodel->setHorizontalHeaderItem(0,new QStandardItem("Goto"));
    changeframemodel->setHorizontalHeaderItem(1,new QStandardItem("Label"));
    Changeframe_Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    animModel->setHorizontalHeaderItem(0,new QStandardItem("Name:"));
    animModel->setHorizontalHeaderItem(1,new QStandardItem("Label:"));
    animModel->setHorizontalHeaderItem(2,new QStandardItem("Image:"));
    Anim_Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    frametime = new QTimer;
    frametime->setInterval(200);
    connect(frametime,SIGNAL(timeout()),this,SLOT(update_Pixmap()));
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
Changeframe_Table = findChild<QTableView*>("Change_List");
Image_Table = findChild<QTableView*>("Image_List");
Anim_Table = findChild<QTableView*>("Anim_List");
ID_Slider = findChild<QSlider*>("ID_slider");
ID_Counter = findChild<QLabel*>("ID_label");
}

void MainWindow::on_last_frame_clicked()
{

}


void MainWindow::on_start_clicked()
{
    if(!frametime->isActive())
        frametime->start();
    else
        frametime->stop();
}


void MainWindow::on_nextframe_clicked()
{

}


void MainWindow::on_ID_slider_sliderMoved(int position)
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
    previewPixmap->addImage(imgname,qxp);

    previewPixmap->play(qxp);

}

void MainWindow::on_Image_List_doubleClicked(const QModelIndex &index)
{
    QString target = index.data().toString();
    previewPixmap->play(getImage(target));
}

QPixmap MainWindow::getImage(QString target)
{
    for(const imgdata id : *imgList){
        if (target == id.name)
            return id.img;
    }
    QPixmap fail;
    return fail;
}

void MainWindow::update_Pixmap()
{
    previewPixmap->update();
    ID_Counter->setText(QString::number(previewPixmap->getID()));
    qDebug() << QString::number(previewPixmap->getID());
}

void MainWindow::on_NewChangeframe_clicked()
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

    if (cf_create.exec())
    {
    previewPixmap->addChangeframe(gotobox->value(),labelbox->value());
    changeframemodel->appendRow(
                QList<QStandardItem*>({new QStandardItem((QString::number(gotobox->value()))),
                                       new QStandardItem(QString::number(labelbox->value()))}));
    }

    delete goto_title;
    delete gotobox;
//  delete space1; Results in a SEGFAULT for some reason
    delete label_title;
    delete labelbox;
    delete accept_button;

}

void MainWindow::on_NewAnimation_clicked()
{

    QDialog na_create(this);
    QHBoxLayout na_layout(&na_create);
    na_create.setWindowTitle("New Animation");

    auto name_title = new QLabel("Name:");
    na_layout.addWidget(name_title);
    auto * namebox = new QLineEdit();
    na_layout.addWidget(namebox);

    auto space1 = new QSpacerItem(2,2);
    na_layout.addItem(space1);

    auto label_title = new QLabel("Label:");
    na_layout.addWidget(label_title);
    auto labelbox = new QSpinBox();
    na_layout.addWidget(labelbox);

//    na_layout.addItem(space1);

    auto img_title = new QLabel("Img:");
    na_layout.addWidget(img_title);
    auto img_select = new QComboBox();
    img_select->addItems(imgNames());
    na_layout.addWidget(img_select);

    auto accept_button = new QPushButton("Accept");
    na_layout.addWidget(accept_button);
    QObject::connect(accept_button,SIGNAL(clicked()),&na_create,SLOT(accept()));

    qDebug() << "Reached...";
    if (na_create.exec())
    {
        qDebug() << "Going...";
        previewPixmap->addImgFrame(namebox->text(),labelbox->value(),
                                   img_select->currentText());
        animModel->appendRow(
                    QList<QStandardItem*>({new QStandardItem((namebox->text())),
                                           new QStandardItem(QString::number(labelbox->value())),
                                           new QStandardItem(img_select->currentText())}));
    }

    delete name_title;
    delete namebox;
    delete label_title;
    delete labelbox;
    delete img_title;
    delete img_select;
    delete accept_button;

}

QList<QString> MainWindow::imgNames()
{
    QList<QString> tmp;
    for (const imgdata dt: *imgList)
        tmp.append(dt.name);
    return tmp;
}
