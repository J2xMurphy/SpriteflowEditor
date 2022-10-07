#include "mainwindow.h"
#include "./ui_mainwindow.h"

QGraphicsScene * EZScene;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug() << "Beginning program";
    ui->setupUi(this);
    populate_pointers();
    setupScene();
    freshenUp();
    setupHeaders();
    qDebug() << "Ending Initialization";
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::freshenUp()
{
    qDebug() << "Initializing";
    frametime = 200;
    imgList = new QList<imgdata>;
    imgmodel = new QStandardItemModel();
    changeframemodel = new QStandardItemModel();
    animModel = new QStandardItemModel();

    Image_Table->setModel(imgmodel);
    Changeframe_Table->setModel(changeframemodel);
    Anim_Table->setModel(animModel);

    frametimer = new QTimer;
    frametimer->setInterval(frametime);
    connect(frametimer,SIGNAL(timeout()),this,SLOT(update_Pixmap()));
    return true;

    ID_Slider->setMaximum(previewPixmap->getMax());
}

void MainWindow::setupScene()
{
    qDebug() << "Setting up Scene";
    EZScene = new QGraphicsScene;
    Pre_View->setScene(EZScene);
    previewPixmap = new Spriteflow();
    setScene(EZScene);
    previewPixmap->sendToScene();
}

void MainWindow::setupHeaders()
{
    qDebug() << "Setting up Headers";
    imgmodel->setHorizontalHeaderItem(0,new QStandardItem(IMAGE_HEADER));
    Image_Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    changeframemodel->setHorizontalHeaderItem(0,new QStandardItem(GOTO_HEADER));
    changeframemodel->setHorizontalHeaderItem(1,new QStandardItem(LABEL_HEADER));
    Changeframe_Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    animModel->setHorizontalHeaderItem(0,new QStandardItem(NAME_HEADER));
    animModel->setHorizontalHeaderItem(1,new QStandardItem(LABEL_HEADER));
    animModel->setHorizontalHeaderItem(2,new QStandardItem(IMAGE_HEADER));
    Anim_Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::populate_pointers(){
Pre_View = findChild<QGraphicsView*>("Preview_View");
Changeframe_Table = findChild<QTableView*>("Change_List");
Image_Table = findChild<QTableView*>("Image_List");
Anim_Table = findChild<QTableView*>("Anim_List");
ID_Slider = findChild<QSlider*>("ID_slider");
ID_Counter = findChild<QLabel*>("ID_label");
Prev_Button = findChild<QPushButton*>("last_frame");
Play_Button = findChild<QPushButton*>("start");
Next_Button = findChild<QPushButton*>("nextframe");
}

void MainWindow::on_last_frame_clicked()
{
    qDebug() << "Decrement Triggered";
    previewPixmap->decrement();
    stopPlayback();
}


void MainWindow::on_start_clicked()
{
    qDebug() << "Start/Stop Triggered";
    if(!frametimer->isActive())
    {
        frametimer->start();
        previewPixmap->play();
    }
    else
    {
        frametimer->stop();
        previewPixmap->stop();
    }
}


void MainWindow::on_nextframe_clicked()
{
    qDebug() << "Incrementing Frame";
    previewPixmap->increment();
    stopPlayback();
}

void MainWindow::on_actionNew_triggered()
{
    qDebug() << "New Triggered";
    delete previewPixmap;
    delete EZScene;
    setupScene();
    imgList->clear();
    imgmodel->clear();
    changeframemodel->clear();
    animModel->clear();
    frametime = 200;
    frametimer->setInterval(frametime);
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
    qDebug() << "Settings Opened";
    previewPixmap->stop();
    QDialog settings(this);
    QVBoxLayout s_layout(&settings);
    settings.setWindowTitle(SETTINGS_TITLE);

    //Section to edit frametime
    QWidget ft(this);
    QHBoxLayout ft_layout(&ft);
    auto f_timer1 = new QLabel(FRAMETIME_TITLE":");
    ft_layout.addWidget(f_timer1);
    auto * timerbox = new QSpinBox();
    ft_layout.addWidget(timerbox);
    auto f_timer2 = new QLabel("ms");
    ft_layout.addWidget(f_timer2);
    s_layout.addWidget(&ft);
    timerbox->setMaximum(9999);
    timerbox->setValue(frametime);
    timerbox->setAlignment(Qt::AlignRight);
    f_timer2->setAlignment(Qt::AlignRight);
    f_timer2->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    //Section to edit timeline cap
    QWidget mx(this);
    QHBoxLayout mx_layout(&mx);
    auto maxLabel = new QLabel(MAXCAP_TITLE":");
    mx_layout.addWidget(maxLabel);
    auto * mxbox = new QSpinBox();
    mx_layout.addWidget(mxbox);
    s_layout.addWidget(&mx);
    mxbox->setAlignment(Qt::AlignRight);

    timerbox->setMaximum(9999);
    timerbox->setValue(frametime);
    mxbox->setMaximum(9999);
    mxbox->setValue(previewPixmap->getMax());

    auto accept_button = new QPushButton(ACCEPT_BUTTON);
    s_layout.addWidget(accept_button);
    QObject::connect(accept_button,SIGNAL(clicked()),&settings,SLOT(accept()));

    if (settings.exec())
    {
        frametime = timerbox->value();
        frametimer->setInterval(frametime);
        previewPixmap->setMax(mxbox->value());
        ID_Slider->setMaximum(previewPixmap->getMax());
    }

    delete f_timer1;
    delete f_timer2;
    delete timerbox;
    delete accept_button;
    previewPixmap->play();
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
    qDebug() << "Opening Image";
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
    qDebug() << "Image Selected";
    QString target = index.data().toString();
    previewPixmap->play(getImage(target));
}

QPixmap MainWindow::getImage(QString target)
{
    for(const imgdata id : *imgList){
        if (target == id.name)
        {
            return id.img;
        }
    }
    QPixmap fail;
    return fail;
}

void MainWindow::update_Pixmap()
{
    qDebug() << "***************MainWindow::update_Pixmap**************";
    previewPixmap->update();
    ID_Counter->setText(QString::number(previewPixmap->getID())+"/"+
                        QString::number(previewPixmap->getMax()));
    qDebug() << QString::number(previewPixmap->getID());
    ID_Slider->setSliderPosition(previewPixmap->getID());
    qDebug() << "************************END**************************";
}

void MainWindow::on_NewChangeframe_clicked()
{
    qDebug() << "New Changeframe";
    QDialog cf_create(this);
    QHBoxLayout cf_layout(&cf_create);
    cf_create.setWindowTitle(CHANGEFRAME_TITLE);

    auto goto_title = new QLabel(GOTO_HEADER":");
    cf_layout.addWidget(goto_title);
    QSpinBox * gotobox = new QSpinBox();
    cf_layout.addWidget(gotobox);

    //Deleting unnecessary, gives segfault
    auto space1 = new QSpacerItem(2,2);
    cf_layout.addItem(space1);

    auto label_title = new QLabel(LABEL_HEADER":");
    cf_layout.addWidget(label_title);
    QSpinBox * labelbox = new QSpinBox();
    cf_layout.addWidget(labelbox);

    auto accept_button = new QPushButton(ACCEPT_BUTTON);
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
    delete label_title;
    delete labelbox;
    delete accept_button;
}

void MainWindow::on_NewAnimation_clicked()
{
    qDebug() << "New Animation";
    QDialog na_create(this);
    QHBoxLayout na_layout(&na_create);
    na_create.setWindowTitle(ANIMATION_TITLE);

    auto name_title = new QLabel(NAME_HEADER":");
    na_layout.addWidget(name_title);
    auto * namebox = new QLineEdit();
    na_layout.addWidget(namebox);

    auto space1 = new QSpacerItem(2,2);
    na_layout.addItem(space1);

    auto label_title = new QLabel(LABEL_HEADER":");
    na_layout.addWidget(label_title);
    auto labelbox = new QSpinBox();
    na_layout.addWidget(labelbox);

    auto img_title = new QLabel(IMAGE_HEADER":");
    na_layout.addWidget(img_title);
    auto img_select = new QComboBox();
    img_select->addItems(imgNames());
    na_layout.addWidget(img_select);

    auto accept_button = new QPushButton(ACCEPT_BUTTON);
    na_layout.addWidget(accept_button);
    QObject::connect(accept_button,SIGNAL(clicked()),&na_create,SLOT(accept()));

    if (na_create.exec())
    {
        previewPixmap->addImgFrame(namebox->text(),labelbox->value(),
                                   img_select->currentText());
        animModel->appendRow(
            QList<QStandardItem*>({new QStandardItem((namebox->text())),
                new QStandardItem(QString::number(labelbox->value())),
                new QStandardItem(img_select->currentText())}));
    }

//CLEANUP
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

void MainWindow::on_ID_slider_valueChanged(int value)
{
    qDebug() << "Slider changed to:"<<value;
    if (!previewPixmap->getPlaying())
    {
        previewPixmap->setID(value);
        stopPlayback();
    }
    ID_Counter->setText(QString::number(previewPixmap->getID())+"/"+
                        QString::number(previewPixmap->getMax()));
}

void MainWindow::on_ID_slider_sliderPressed()
{
    qDebug() << "Slider Pressed";
    previewPixmap->setID(ID_Slider->value());
    stopPlayback();
}


void MainWindow::on_ID_slider_actionTriggered(int action)
{
    qDebug() <<"Slider Action:" << action << ID_Slider->value();
    if (action == 3 || action == 4)
    {
        previewPixmap->setID(ID_Slider->value());
        stopPlayback();
    }
}

void MainWindow::stopPlayback()
{
    qDebug() << "Stopping Playback";
    frametimer->stop();
    ID_Counter->setText(QString::number(previewPixmap->getID())+"/"+
                        QString::number(previewPixmap->getMax()));
    previewPixmap->stop();
    Play_Button->setChecked(false);
}


void MainWindow::on_ID_slider_sliderMoved(int position)
{
    qDebug() << "Slider Moved";
    previewPixmap->setID(position);
    stopPlayback();
}

