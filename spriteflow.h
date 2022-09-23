#ifndef SPRITEFLOW_H
#define SPRITEFLOW_H

#include <QString>
#include <QList>
#include <QPixmap>
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsPixmapItem>

extern QGraphicsScene * EZScene;

struct Anim
{
    QString name;
    int start;
    int ID;
};

struct ChangeFrame
{
    int goTo;
    int label;
};

struct ImgFrame
{
    int ID;
    QPixmap * img;
};

class Spriteflow : public QGraphicsPixmapItem{
//    Q_OBJECT
    QList<Anim> anim_list;
    QList<ChangeFrame> changeframes; //Consider making a hash map for easier searching
    QList<ImgFrame> imgframes;
    QList<QPixmap> images;

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
//    spriteflow(RenderObject);
    void update();                 //Call per frame to update image
    void addAnim(QString,int,int); //Name points to starting frame ID
    void addChangeframe(int,int);  //When A frame is reached go to B frame
    void addImgFrame(int,QPixmap); //When A frame is reached, set img
    void addImgFrame(int,int);     //When A frame is reached, set list img
    void addImage(QPixmap);        //Adds img into saved list
    void play(QPixmap);            //Plays anim from list by name
    void play(int);                //Sets current anim to int
    void inheritPos(int*, int*);   //Inherits cordinates from parent
    bool sendToScene();
    bool sendToScene(QGraphicsScene*);
    int contains(QString);
    int contains(int);
    int isChangeFrame();
    void setImgFrame(int);
    QList<ChangeFrame>* getChangeFrame();
    QList<ImgFrame>* getImageFrame();
    QList<Anim>* getAnim();

    float getID() const;

public slots:
    void updateparams();
};

bool setScene(QGraphicsScene*);

#endif // SPRITEFLOW_H
