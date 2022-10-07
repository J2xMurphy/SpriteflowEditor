#include "spriteflow.h"

bool setScene(QGraphicsScene* scene)
{
//  Checks if passed in scene actually exists.
    if (scene == nullptr)
    return 0;
    EZScene = scene;
    return 1;
}

float Spriteflow::getID()
{
    return ID;
}

float Spriteflow::getMax()
{
    return max;
}

void Spriteflow::setMax(float newMax)
{
    max = newMax;
}

void Spriteflow::setID(float newID)
{
    qDebug() << "--Spriteflow::Setting ID to:" << newID;
    ID = newID;
    if (ID<0)
    {
        ID=this->max;
    }
    if (ID>max)
    {
        ID=0;
    }

    int tmp = isChangeFrame();
    tmp = isAnim();
    if (tmp > -1)
        ani(tmp);
}

bool Spriteflow::getPlaying() const
{
    return playing;
}

Spriteflow::Spriteflow()
{
    //If no parent is passed in, creates usable variables
    if (x==nullptr && y==nullptr)
    {
        x = new int(0);
        y = new int(0);
        linked = false;
    }
    ID = 0;
    rate = 1;
    max = 100;
    images = new QList<imgPage>;
}

Spriteflow::Spriteflow(int * p1, int * p2)
{
    //only positional variables are passed in, so linked is true
    inheritPos(p1,p2);
    linked = true;
    max=100;
    images = new QList<imgPage>;
}

void Spriteflow::update()
{
    qDebug() << "*****Beginning update*****";
    //Only run when playing is true
    if (playing == false)
    {
        return;
    }
    //Change this to be affected by rate
    ID++;
    qDebug() <<"Updating to " << ID;
    //Checks if ID should be jumped
    int tmp = isChangeFrame();
    if (tmp > -1)
        play(tmp);

    //Checks if the current picture should be changed
    tmp = isAnim();
    if (tmp > -1)
        ani(tmp);

    //Resets to 0 if cap is hit
    if (ID >= max)
    {
        ID = 0;
    }
    //Make it to update parent xy
    qDebug() << "*****Ending update*****";
}

void Spriteflow::addChangeframe(int cgoto, int clabel)
{
    //Just adds the args to the changeframe list
    changeframes.append({cgoto,clabel});
    qDebug() << "Added Changeframe";
}

void Spriteflow::addImgFrame(QString name,int imgID, QPixmap img)
{
    //Adds the img to the imglist, then adds the image to imagefram
    images->append({name,new QPixmap(img)});
    assert(images->size()>0);
    imgframes.append({name,imgID,(images[images->size()-1].data()->img)});
    qDebug() << "Added Animation";
}

void Spriteflow::addImgFrame(QString name, int imgID, QString img)
{
    //Finds the image in the images list, then adds to imagframe
    imgframes.append({name,imgID,findImg(img)});
    qDebug() << "Added Animation";
}

void Spriteflow::addImgFrame(QString name,int imgID, int ref)
{
    //Adds to imageframe based on images array index
    imgframes.append({name,imgID,images[ref].data()->img});
    qDebug() << "Added Animation";
}

void Spriteflow::addImage(QString name,QPixmap img)
{
    //Adds the image to images list for later use
    images->append({name,new QPixmap(img)});
    qDebug() << "Added Image";
}

void Spriteflow::play(QPixmap img)
{
    //Resumes playback, and sets the current image to passed in image
    qDebug() << "Playing image";
    playing = true;
    this->setPixmap(img);
}

void Spriteflow::play(int IID)
{
    //Resumes playback, and sets the frame ID to argument and associated images
    //Possibly unnecessary, as update basically does what setImgFrame(IID) does
    if (ID==-1)
    {
        return;
    }
    qDebug() << "Playing index";
    playing = true;
    ID = IID;
    setImgFrame(IID);
}

void Spriteflow::play(QString name)
{
    //Resumes playback, and sets the current image to the saved QPixmap by name
    qDebug() << "Playing name";
    playing = true;
    this->setPixmap(*findImg(name));
}

void Spriteflow::play()
{
    //Resumes playback, thats it.
    playing = true;
}

void Spriteflow::stop()
{
    //Stops playback, thats it.
    playing = false;
}

void Spriteflow::inheritPos(int * p1, int *p2)
{
    //Deletes x and y if I had to make my own
    if (linked == false)
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
    //Checks if global scene has been added, then adds itself to it
    if (EZScene == nullptr)
    {
        return 0;
    }
    EZScene->addItem(this);
    return 1;
}

bool Spriteflow::sendToScene(QGraphicsScene *scene)
{
    //Given aspecific scene, will add myself to it
    scene->addItem(this);
    return 0;
}


int Spriteflow::isChangeFrame()
{
    //Checks frame ID against all goto statements, and sets returns the ID on hit.
    qDebug() << "Checking changeframe:" << ID << changeframes.size();
    for (ChangeFrame c:changeframes){
        if (ID==c.goTo)
        {
            qDebug() << "Changeframe hit:" << c.goTo << c.label;
            return c.label;
        }
    }
    qDebug() << "Changeframe miss";
    return -1;
}

int Spriteflow::isAnim()
{
    //Checks all imageframes to see if the frame index matches. Returns array index isf found
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
    //Searches the imageframes for a ID hit, and sets the current pixmap to associated
    qDebug() << "Setting Image";
    for (ImgFrame fimg:imgframes)
    {
        if (fimg.ID == IDD)
        {
            QPixmap * ptr = fimg.img;
            this->setPixmap(*ptr);
        }
    }
}

void Spriteflow::ani(int index)
{
    //sets image to array index, fails if too large
    qDebug() << "Setting anim" << index;
    if (index > imgframes.size())
    {
        return;
    }
    this->setPixmap(*imgframes.at(index).img);
}

QPixmap *Spriteflow::findImg(QString target)
{
    //Compares imgpages by name vs target, then returns the stored image on hit
    qDebug() << "Finding " << target;
    for (int i = 0; i<images->size();++i)
    {
        qDebug() << "Comparing " << target << " and " << images->at(i).name << i;
        if (target == images->at(i).name)
        {
            qDebug() << "Found" << target << images->at(i).name << i << " " << images->size()-1;
            QPixmap * tmp = images->at(i).img;
            return tmp;
        }
    }
    qDebug() << "Image Unfound" ;
    //Upon failing, returns a new pixmap of dummy data
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

void Spriteflow::decrement(int i)
{
    stop();
    ID-=i;
    if (ID<0)
    {
        ID=this->max;
    }
}

void Spriteflow::increment(int i)
{
    stop();
    ID+=i;
    if (ID>max)
    {
        ID=0;
    }
}

void Spriteflow::updateparams()
{
    //Pulls data from the parent o update the pixmap position.
    setPos(qreal(*x),qreal(*y));
}
