#include "spriteflow.h"

bool setScene(QGraphicsScene* scene)
{
    if (scene == nullptr)
    return 0;

    EZScene = scene;
    return 1;
}

spriteflow::spriteflow()
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

spriteflow::spriteflow(int * p1, int * p2)
{
    inheritPos(p1,p2);
    linked = 1;
}

void spriteflow::update()
{
    if (playing){
        ID++;
    }
}

void spriteflow::addAnim(QString name, int sframe, int animID)
{
    anim_list.append({name,sframe,animID});
}

void spriteflow::addChangeframe(int cgoto, int clabel)
{
    changeframes.append({cgoto,clabel});
}

void spriteflow::addImgFrame(int imgID, QPixmap img)
{
    imgframes.append({imgID,img});
}

void spriteflow::addImgFrame(int imgID, int ref)
{
    imgframes.append({imgID,images[ref]});
}

void spriteflow::addImage(QString img)
{
    images.append(img);
}

void spriteflow::play(QString img)
{
    playing = true;
}

void spriteflow::play(int IID)
{
    playing = true;
    ID = IID;
}

void spriteflow::inheritPos(int * p1, int *p2)
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

bool spriteflow::sendToScene()
{
    if (EZScene == nullptr)
        return 0;
    EZScene->addItem(this);
    return 1;
}

bool spriteflow::sendToScene(QGraphicsScene *scene)
{
    scene->addItem(this);
    return 0;
}

int spriteflow::contains(QString target)
{
    for (std::tuple<QString,int,int> element: anim_list)
    {
        if (target.compare(std::get<QString>(element)))
            return std::get<1>(element);
    }
    return -1;
}

int spriteflow::contains(int index)
{
    if (index>anim_list.size())
        return -1;
    return  std::get<1>(anim_list[index]);
}

void spriteflow::updateparams()
{
    setPos(qreal(*x),qreal(*y));
}
