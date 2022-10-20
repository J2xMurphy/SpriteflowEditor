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

//    int tmp = isChangeFrame();
    int tmp = isAnim();
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
    images = new QList<imgPage*>;
    nullimage = new QPixmap(":/new/prefix1/Missing.png");
    blankPage = new imgPage{"",nullimage};
}

Spriteflow::Spriteflow(int * p1, int * p2)
{
    //only positional variables are passed in, so linked is true
    inheritPos(p1,p2);
    linked = true;
    max=100;
    images = new QList<imgPage*>;
    nullimage = new QPixmap(":/new/prefix1/Missing.png");
    blankPage = new imgPage{"",nullimage};
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

void Spriteflow::addChangeFrame(int cgoto, int clabel)
{
    //Just adds the args to the changeframe list
    changeframes.append({cgoto,clabel});
    qDebug() << "Added Changeframe";
}

void Spriteflow::editChangeFrame(int index, int cgoto, int clabel)
{
    //Reaplces all parameters of the changeframe at index with given arguments
    changeframes.replace(index,{cgoto,clabel});
    qDebug() << "Edited Changeframe";
}

void Spriteflow::removeChangeFrame(int index)
{
    changeframes.remove(index);
}

void Spriteflow::addImgFrame(QString name,int imgID, QPixmap img)
{
    //Adds the img to the imglist, then adds the image to imagefram
    images->append(new imgPage{name,new QPixmap(img)});
    assert(images->size()>0);
    imgframes.append({name,imgID,&(images->at(images->size()-1)->img)});
    qDebug() << "Added new Animation to book";
}

void Spriteflow::addImgFrame(QString name, int imgID, QString img)
{
    //Finds the image in the images list, then adds to imagframe
    imgframes.append({name,imgID,findImg(img)});
    qDebug() << "Added Animation from book";
}

void Spriteflow::editImgFrame(int index,QString name, int imgID, QString img)
{
    //Replaces all the parameters of the image frame with given arguments
    imgframes.replace(index,{name,imgID,findImg(img)});
    qDebug() << "Edited Animation";
}

void Spriteflow::removeImgFrame(int index)
{
    imgframes.remove(index);
}

void Spriteflow::addImgFrame(QString name,int imgID, int ref)
{
    //Adds to imageframe based on images array index
    imgframes.append({name,imgID,&images->at(ref)->img});
    qDebug() << "Added Animation";
}

void Spriteflow::addImage(QString name,QPixmap img)
{
    //Adds the image to images list for later use
    images->append(new imgPage{name,new QPixmap(img)});
    qDebug() << "Added Image";
}

void Spriteflow::removeImage(int index)
{
    //Deletes the image and page entry
    qDebug() << "Deleting image" << index;
    images->at(index)->img = nullimage;
    images->removeAt(index);
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
    this->setPixmap(**findImg(name));
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
    for (const ImgFrame &fimg : *&imgframes){
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
    for (const ImgFrame &fimg:imgframes)
    {
        if (fimg.ID == IDD)
        {
            if (((QPixmap*)&fimg.img)==nullptr)
            {
                this->setPixmap(*nullimage);
                return;
            }
            this->setPixmap(**fimg.img);
        }
    }
}

void Spriteflow::ani(int index)
{
    //Sets image to array index, fails if too large
    qDebug() << "Setting anim" << index;
    if (index > imgframes.size())
    {
        return;
    }
    //Must handle what happens in case of deleted image
    QPixmap * tmp = *imgframes.at(index).img;
    if (tmp->isNull())
    {
        qDebug() << "Image was not found, possibly deleted!";
        this->setPixmap(*nullimage);
        return;
    }
    this->setPixmap(**imgframes.at(index).img);
}

QPixmap** Spriteflow::findImg(QString target)
{
    //Compares imgpages by name vs target, then returns the stored image on hit
    qDebug() << "Finding " << target;
    for (int i = 0; i<images->size();++i)
    {
        qDebug() << "Comparing " << target << " and " << images->at(i)->name << i;
        if (target == images->at(i)->name)
        {
            qDebug() << "Found" << target << images->at(i)->name << i << " " << images->size()-1;
//            QPixmap * tmp = images->at(i)->img;
            return &images->at(i)->img;
        }
    }
    qDebug() << "Image Unfound" ;
    //Upon failing, returns a new pixmap of dummy data
    return (&nullimage);
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
