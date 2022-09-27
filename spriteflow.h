#ifndef SPRITEFLOW_H
#define SPRITEFLOW_H

#include <QString>
#include <QList>
#include <QPixmap>
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <iostream>

extern QGraphicsScene * EZScene;

//struct Anim
//{
//    QString name;
//    int start;
//    int ID;
//};

struct ChangeFrame
{
    int goTo;
    int label;
};

struct ImgFrame
{
    QString name;
    int ID;
    QPixmap * img;
};

struct imgPage
{
    QString name;
    QPixmap * img;
};

class Spriteflow : public QGraphicsPixmapItem{
//    Q_OBJECT
//    QList<Anim> anim_list; Functionality moved to imgframes
    QList<ChangeFrame> changeframes; //Consider making a hash map for easier searching
    QList<ImgFrame> imgframes;
    QList<imgPage> * images;

    float ID, rate;
    int *x=nullptr, *y=nullptr, *depth=nullptr;
    bool linked = false;
    qreal scale;
    bool playing = true;
    //TODO
    //RenderObject * parent;
    //This would require a specific parent to function, which is not intended.
public:
    Spriteflow();
    Spriteflow(int*, int*);        //Constructor to auto inherit position
    void update();                 //Call per frame to update image
    void addChangeframe(int,int);  //When A frame is reached go to B frame
    void addImgFrame(QString,int,QPixmap); //When A frame is reached, set img
    void addImgFrame(QString,int,QString); //When A frame is reached, set img
    void addImgFrame(QString,int,int);     //When A frame is reached, set list img
    void addImage(QString,QPixmap);        //Adds img into saved list
    void play(QPixmap);            //Plays anim from list by name
    void play(int);                //Sets current anim to int
    void play(QString);
    void inheritPos(int*, int*);   //Inherits cordinates from parent
    bool sendToScene();
    bool sendToScene(QGraphicsScene*);
    int isChangeFrame();
    int isAnim();
    void setImgFrame(int);
    void ani(int);
    QPixmap* findImg(QString);
    QList<ChangeFrame>* getChangeFrame();
    QList<ImgFrame>* getImageFrame();

//    spriteflow(RenderObject);

//    void addAnim(QString,int,int); //Name points to starting frame ID
//    int contains(QString);
//    int contains(int);
//    QList<Anim>* getAnim();

    float getID();

public slots:
    void updateparams();
};

bool setScene(QGraphicsScene*);

#endif // SPRITEFLOW_H
