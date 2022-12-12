#include "mainwindow.h"
#include "./ui_mainwindow.h"

QGraphicsScene * EZScene;
imgdata::operator QByteArray()
{
    QByteArray qb;
    for (char c :name.toStdString())
    {
        qb.append(c);
    }
    int wat = (img.height()*img.width()*img.depth())/8;
    std::cout << img.height() << " * " << img.width() << " * " << img.depth()
              << " /8 = " << wat << std::endl;
    qb.append(wat);
    for (int i = 0;i< 16 ;i++)
    {
        qb.append("a");
    }
    return qb;
}

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
    openedFile = "";
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
    setupHeaders();
}

void MainWindow::on_actionOpen_triggered()
{
    qDebug("Opening custom file");
    QString dir;
    dir = QFileDialog::getOpenFileName(this,
        "Open...", "", "Filetypes (*.sfs)");
    openFile(dir);
}

void MainWindow::on_actionSave_triggered()
{
    if (openedFile == "")
    {
        return;
    }
    saveFile(openedFile);
}

void MainWindow::on_actionSave_As_triggered()
{
    openedFile = QFileDialog::getSaveFileName(this,"Save as...","","*.sfs");
    saveFile(openedFile);
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
    QList<QString> dir;
    QFileDialog op;
    op.setFileMode(QFileDialog::ExistingFiles);
    dir = op.getOpenFileNames(this,
        "Open...", "", "(*.jpg *.png *.jpeg)");

    for(const QString &file:dir)
    {
    openImage(file);
    }
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
    previewPixmap->play(target);
    EZScene->setSceneRect(previewPixmap->sceneBoundingRect());
}

QPixmap MainWindow::getImage(QString target)
{
    qDebug() << "Finding image:" << target;
    for(imgPage * ip : *previewPixmap->getImagePages()){
        if (target == ip->name)
        {
            qDebug() << "Found" << target;
            return *ip->img;
        }
    }

    qDebug() << "Unfound " << target;
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
        addChangeFrame(gotobox->value(),labelbox->value());
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
            QList<QStandardItem*>({
                new QStandardItem((namebox->text())),
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

void MainWindow::on_Anim_List_doubleClicked(const QModelIndex &index)
{
    qDebug() << "Edit Animation";
    QDialog na_create(this);
    QHBoxLayout na_layout(&na_create);
    na_create.setWindowTitle(EDIT_ANIMATION_TITLE);

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
    namebox->setText(animModel->item(index.row(),0)->text());
    labelbox->setValue(animModel->item(index.row(),1)->text().toInt());
    img_select->setCurrentText(animModel->item(index.row(),2)->text());
    if (na_create.exec())
    {
        animModel->item(index.row(),0)->setText(namebox->text());
        animModel->item(index.row(),1)->setText(QString::number(labelbox->value()));
        animModel->item(index.row(),2)->setText(img_select->currentText());
        previewPixmap->editImgFrame(index.row(),namebox->text(),
                                    labelbox->value(),img_select->currentText());
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

void MainWindow::on_Change_List_doubleClicked(const QModelIndex &index)
{

    qDebug() << "Editing Changeframe";
    QDialog cf_create(this);
    QHBoxLayout cf_layout(&cf_create);
    cf_create.setWindowTitle(EDIT_CHANGEFRAME_TITLE);

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

    gotobox->setValue(changeframemodel->item(index.row(),0)->text().toInt());
    labelbox->setValue(changeframemodel->item(index.row(),1)->text().toInt());

    if (cf_create.exec())
    {
        changeframemodel->item(index.row(),0)->setText(QString::number(gotobox->value()));
        changeframemodel->item(index.row(),1)->setText(QString::number(labelbox->value()));
        previewPixmap->editChangeFrame(index.row(),gotobox->value(),labelbox->value());
    }

    delete goto_title;
    delete gotobox;
    delete label_title;
    delete labelbox;
    delete accept_button;
}

void MainWindow::on_delete_Button_clicked()
{
    qDebug() << "delete clicked";
    if (Changeframe_Table->hasFocus())
    {
        qDebug() << "Changeframe table is focused" << Changeframe_Table->selectionModel()->currentIndex().row();
        removeChangeframeEntry(Changeframe_Table->selectionModel()->currentIndex().row());
    }
    else if (Image_Table->hasFocus())
    {
        qDebug() << "Image table is focused" << Image_Table->selectionModel()->currentIndex().row();
        removeImageEntry(Image_Table->selectionModel()->currentIndex().row());
    }
    else if (Anim_Table->hasFocus())
    {
        qDebug() << "Anim table is focused" << Anim_Table->selectionModel()->currentIndex().row();;
        removeAnimEntry(Anim_Table->selectionModel()->currentIndex().row());
    }
    else
    {
        qDebug() << "No relevant focus";
    }
}

void MainWindow::removeChangeframeEntry(int row)
{
    changeframemodel->takeRow(row);
    previewPixmap->removeChangeFrame(row);
}

void MainWindow::removeAnimEntry(int row)
{
    animModel->takeRow(row);
    previewPixmap->removeImgFrame(row);
}

void MainWindow::removeImageEntry(int row)
{
   imgmodel->takeRow(row);
   previewPixmap->removeImage(row);
}

void MainWindow::addChangeFrame(int gt, int lbl)
{
    previewPixmap->addChangeFrame(gt,lbl);
    changeframemodel->appendRow(
        QList<QStandardItem*>({new QStandardItem((QString::number(gt))),
            new QStandardItem(QString::number(lbl))}));
}

void MainWindow::addImgFrame(QString name, int ID, QString image)
{
    previewPixmap->addImgFrame(name,ID,image);
    animModel->appendRow(
        QList<QStandardItem*>({
            new QStandardItem(name),
            new QStandardItem(ID),
            new QStandardItem(image)}));
}

short MainWindow::saveFile(QString fn)
{
    qDebug() << "Saving file to location: " << fn;
    if (fn.isEmpty())
    {
        return -1;
    }
    QFile file(fn);
    if (file.open(QIODevice::WriteOnly) == false)
    {
        qDebug() << "Error opening file";
        return -2;
    }

    QDataStream output(&file);
    QByteArray qb;
    QBuffer buf(&qb);
    buf.open(QIODevice::WriteOnly);

    buf.write("<!AS->");
    for (QStandardItem* aI : animModel->findItems("",Qt::MatchContains))
    {
        qDebug() << aI->index().siblingAtColumn(0).data().toString()
                 << aI->index().siblingAtColumn(1).data().toString()
                 << aI->index().siblingAtColumn(2).data().toString();
        buf.write("[");
        buf.write(QByteArray::fromStdString(
                      aI->index().siblingAtColumn(0).data().toString().toStdString()));
        buf.write(",");
        buf.write(QByteArray::number(aI->index().siblingAtColumn(1).data().toInt()));
        buf.write(":");
        buf.write(QByteArray::fromStdString(
                      aI->index().siblingAtColumn(2).data().toString().toStdString()));
        buf.write("]");
    }
    buf.write("<!AE->");

    buf.write("<!CS->");
    for (const ChangeFrame cf: *previewPixmap->getChangeFrame())
    {
        buf.write("[");
        qDebug() << cf.goTo << cf.label;
        buf.write(QByteArray::number(cf.goTo));
        buf.write(",");
        buf.write(QByteArray::number(cf.label));
        buf.write("]");
    }
    buf.write("<!CE->");

    buf.write("<!!I+>");
    for (const imgdata &id: *imgList)
    {
        qDebug() << buf.size();
        buf.write("<!IS->");
        buf.write(QByteArray::fromStdString(id.name.toStdString()+","));
        id.img.save(&buf,"PNG");
        buf.write("<!IE->");
        qDebug() << buf.size();
    }
    buf.write("<!!I->");
    output << buf.buffer();
    file.close();
    return 1;
}

short MainWindow::openFile(QString dir){
    QList<QString> leftover = previewPixmap->openFile(dir);
    openedFile = dir;
    int tmp = 0;
    for (const ImgFrame &eyef : *previewPixmap->getImageFrame())
    {
        animModel->appendRow(
            QList<QStandardItem*>({
                new QStandardItem(eyef.name),
                new QStandardItem(QString::number(eyef.ID)),
                new QStandardItem(leftover[tmp])}));
        tmp++;
    }
    for (ChangeFrame cf:*previewPixmap->getChangeFrame())
    {
        changeframemodel->appendRow({
                new QStandardItem(QString::number(cf.goTo)),
                new QStandardItem(QString::number(cf.label))});
    }
    for (imgPage * ip : *previewPixmap->getImagePages())
    {
        imgmodel->appendRow(new QStandardItem(ip->name));
        imgList->append({ip->name,*ip->img});
    }

    return 1;
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox * QMB = new QMessageBox;
    QMB->setWindowTitle(ABOUT_TITLE);
    QMB->setText(ABOUT);
    QMB->exec();

    delete QMB;
}
