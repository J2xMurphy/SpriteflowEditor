#include "spriteflow.h"

bool setScene(QGraphicsScene* scene)
{
    if (scene == nullptr)
    return 0;
    EZScene = scene;
    return 1;
}

float Spriteflow::getID() const
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
}

void Spriteflow::addAnim(QString name, int sframe, int animID)
{
    anim_list.append({name,sframe,animID});
}

void Spriteflow::addChangeframe(int cgoto, int clabel)
{
    changeframes.append({cgoto,clabel});
}

void Spriteflow::addImgFrame(int imgID, QPixmap img)
{
    images.append(img);
    assert(images.size()>0);
    imgframes.append({imgID,&images[images.size()-1]});
}

void Spriteflow::addImgFrame(int imgID, int ref)
{
    imgframes.append({imgID,&images[ref]});
}

void Spriteflow::addImage(QPixmap img)
{
    images.append(img);
}

void Spriteflow::play(QPixmap img)
{
    playing = true;
    this->setPixmap(img);

}

void Spriteflow::play(int IID)
{
    playing = true;
    ID = IID;
    setImgFrame(IID);
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

int Spriteflow::contains(QString target)
{
    for (const Anim &element: anim_list)
    {
        if (target.compare(element.name))
            return element.start;
    }
    return -1;
}

int Spriteflow::contains(int index)
{
    if (index>anim_list.size())
        return -1;
    return  (anim_list[index]).start;
}

int Spriteflow::isChangeFrame()
{
    for (ChangeFrame c:changeframes){
        if (ID==c.goTo)
            return c.label;
    }
    return -1;
}

void Spriteflow::setImgFrame(int IDD)
{
    for (const ImgFrame fimg:imgframes)
    {
        if (fimg.ID == IDD){
            QPixmap * ptr = fimg.img;
            this->setPixmap(*ptr);
        }
    }
}

QList<ChangeFrame>* Spriteflow::getChangeFrame()
{
    return &changeframes;
}

QList<ImgFrame>* Spriteflow::getImageFrame()
{
    return &imgframes;
}

QList<Anim>* Spriteflow::getAnim()
{
    return &anim_list;
}

void Spriteflow::updateparams()
{
    setPos(qreal(*x),qreal(*y));
}
