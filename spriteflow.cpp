#include "spriteflow.h"

bool setScene(QGraphicsScene* scene)
{
    if (scene == nullptr)
    return 0;
    EZScene = scene;
    return 1;
}

float Spriteflow::getID()
{
    return ID;
}

Spriteflow::Spriteflow()
{
    if (x==nullptr && y==nullptr)
    {
        x = new int(0);
        y = new int(0);
        linked = true;
    }
    ID = 0;
    rate = 1;
    images = new QList<imgPage>;
}

/*spriteflow::spriteflow(RenderObject*)
 * {
 * parent = RenderObject;
 * x = RenderObject->*x;
 * y = RenderObject->*y;
 * linked = 1;
 * }
*/

Spriteflow::Spriteflow(int * p1, int * p2)
{
    inheritPos(p1,p2);
    linked = 1;
    images = new QList<imgPage>;
}

void Spriteflow::update()
{
    if (!playing){
        return;
    }
    ID++;
    int tmp = isChangeFrame();
    if (tmp > -1)
        play(tmp);
    tmp = isAnim();
    if (tmp > -1)
        ani(tmp);
}

void Spriteflow::addChangeframe(int cgoto, int clabel)
{
    changeframes.append({cgoto,clabel});
    qDebug() << "Added Changeframe";
}

void Spriteflow::addImgFrame(QString name,int imgID, QPixmap img)
{
    images->append({name,new QPixmap(img)});
    assert(images->size()>0);
    imgframes.append({name,imgID,(images[images->size()-1].data()->img)});
    qDebug() << "Added Animation";
}

void Spriteflow::addImgFrame(QString name, int imgID, QString img)
{
    imgframes.append({name,imgID,findImg(img)});
    qDebug() << "Added Animation";
}

void Spriteflow::addImgFrame(QString name,int imgID, int ref)
{
    imgframes.append({name,imgID,images[ref].data()->img});
    qDebug() << "Added Animation";
}

void Spriteflow::addImage(QString name,QPixmap img)
{
    images->append({name,new QPixmap(img)});
    qDebug() << "Added Image";
}

void Spriteflow::play(QPixmap img)
{
    qDebug() << "Playing image";
    playing = true;
    this->setPixmap(img);
}

void Spriteflow::play(int IID)
{
    qDebug() << "Playing int";
    playing = true;
    ID = IID;
    setImgFrame(IID);
}

void Spriteflow::play(QString name)
{
    qDebug() << "Playing name";
    this->setPixmap(*findImg(name));
}

void Spriteflow::inheritPos(int * p1, int *p2)
{
    if (linked)
    {
        delete x;
        delete y;
    }
    x = p1;
    y = p2;
    linked = true;
}

bool Spriteflow::sendToScene()
{
    if (EZScene == nullptr)
        return 0;
    EZScene->addItem(this);
    return 1;
}

bool Spriteflow::sendToScene(QGraphicsScene *scene)
{
    scene->addItem(this);
    return 0;
}


int Spriteflow::isChangeFrame()
{
    for (ChangeFrame c:changeframes){
        if (ID==c.goTo)
            return c.label;
    }
    return -1;
}

int Spriteflow::isAnim()
{
    qDebug() << "Checking anim: " << ID << imgframes.size();
    int tmp = -1;
    for (ImgFrame fimg : *&imgframes){
        tmp++;
        if (ID==fimg.ID)
        {
            qDebug() << "Animmatch" << ID;
            return tmp;
        }
    }
    return -1;
}

void Spriteflow::setImgFrame(int IDD)
{
    qDebug() << "Setting Image";
    for (ImgFrame fimg:imgframes)
    {
        if (fimg.ID == IDD){
            QPixmap * ptr = fimg.img;
            this->setPixmap(*ptr);
        }
    }
}

void Spriteflow::ani(int index)
{
    qDebug() << "Setting anim" << index;
    this->setPixmap(*imgframes.at(index).img);
}

QPixmap *Spriteflow::findImg(QString target)
{
    qDebug() << "Finding " << target;
    for (int i = 0; i<images->size();++i)
    {
        qDebug() << "Comparing " << target << " and " << images->at(i).name << i;
        if (target == images->at(i).name){
            qDebug() << "Found";
            qDebug() << target << images->at(i).name << i << " " << images->size()-1;
            QPixmap * tmp = images->at(i).img;
            qDebug() << "Ending";
            return tmp;}
    }
    qDebug() << "Image Unfound" ;
    return (new QPixmap("Dummy"));
}

QList<ChangeFrame>* Spriteflow::getChangeFrame()
{
    return &changeframes;
}

QList<ImgFrame>* Spriteflow::getImageFrame()
{
    return &imgframes;
}

void Spriteflow::updateparams()
{
    setPos(qreal(*x),qreal(*y));
}

//********************************************
//Animlist functions
//********************************************

//int Spriteflow::contains(QString target)
//{
//    for (const Anim &element: anim_list)
//    {
//        if (target.compare(element.name))
//            return element.start;
//    }
//    return -1;
//}

//int Spriteflow::contains(int index)
//{
//    if (index>anim_list.size())
//        return -1;
//    return  (anim_list[index]).start;
//}

//QList<Anim>* Spriteflow::getAnim()
//{
//    return &anim_list;
//}

//void Spriteflow::addAnim(QString name, int sframe, int animID)
//{
//    anim_list.append({name,sframe,animID});
//}
