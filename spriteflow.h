#ifndef SPRITEFLOW_H
#define SPRITEFLOW_H

#include <QString>
#include <QList>
#include <QPixmap>
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsItem>

QGraphicsScene * EZScene = nullptr;

class spriteflow : public QGraphicsItem{
    Q_OBJECT
    QList<std::tuple<QString,int,int>> anim_list;
    QList<std::tuple<int,int>> changeframes;
    QList<std::tuple<int,QPixmap>> imgframes;
    QList<QPixmap> images;
    float ID, rate;
    int *x=nullptr, *y=nullptr, *depth=nullptr;
    bool linked = false;
    qreal scale;
    bool playing = true;
    //TODO
    //RenderObject * parent;
    //This would require a specific parent to function, which is not intended.

    spriteflow();
    spriteflow(int*, int*);        //Constructor to auto inherit position
//    spriteflow(RenderObject);
    void update();                 //Call per frame to update image
    void addAnim(QString,int,int); //Name points to starting frame ID
    void addChangeframe(int,int);  //When A frame is reached go to B frame
    void addImgFrame(int,QPixmap); //When A frame is reached, set img
    void addImgFrame(int,int);     //When A frame is reached, set list img
    void addImage(QString);        //Adds img into saved list
    void play(QString);            //Plays anim from list by name
    void play(int);                //Sets current anim to int
    void inheritPos(int*, int*);   //Inherits cordinates from parent
    bool sendToScene();
    bool sendToScene(QGraphicsScene*);
    int contains(QString);
    int contains(int);

public slots:
    void updateparams();
};

bool setScene(QGraphicsScene*);

#endif // SPRITEFLOW_H
