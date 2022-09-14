#ifndef SPRITEFLOW_H
#define SPRITEFLOW_H

#include <QString>
#include <QList>
#include <QPixmap>

class spriteflow{
    QList<std::tuple<QString,int,int>> anim_list;
    QList<std::tuple<int,int>> changeframes;
    QList<std::tuple<int,QPixmap>> imgframes;
    QList<QPixmap> images;

    spriteflow();
    void update();                  //Call per frame to update image
    void add_anim(QString,int,int); //Name points to starting frame ID
    void add_changeframe(int,int);  //When A frame is reached go to B frame
    void add_imgframe(int,QPixmap); //When A frame is reached, set img
    void add_imgframe(int,int);     //When A frame is reached, set list img
    void add_image(QString);        //Adds img into saved list
    void play(QString);             //Plays anim from list by name
    void play(int);                 //Sets current anim to int



};

#endif // SPRITEFLOW_H
