#ifndef SPRITEFLOW_H
#define SPRITEFLOW_H

#include <QString>
#include <QList>
#include <QPixmap>
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <iostream>
#include <QFile>
#include <QByteArray>

extern QGraphicsScene * EZScene;

//When goto is hit, refers the class to change ID to label
struct ChangeFrame
{
    int goTo;
    int label;
};

//Associates ID to a name, and an ID to an image.
struct ImgFrame
{
    QString name;
    int ID;
    QPixmap ** img;
};

//Associates a name to an image. These actually should hold allocated data.
struct imgPage
{
    QString name;
    QPixmap * img;
};

class Spriteflow : public QGraphicsPixmapItem{
    QList<ChangeFrame> changeframes; //List of frames to jump to from a set frame ID
    QList<ImgFrame> imgframes;       //List of identifiers and image to show on ID
    QList<imgPage*> * images;         //List of images in the sprite

    float ID;             //The current frame index
    float max;            //The maximum value before looping
    float rate;           //The amount the index changes per update
    int *x=nullptr;       //The x position. Can be polled from parent
    int *y=nullptr;       //The y position. Can be polled from parent
    int *depth=nullptr;   //The depth determines what it is drawn on top of
    bool linked = false;  //Refers to whether there is a parent attatched
    qreal scale;          //Makes the image larger or smaller
    bool playing = true;  //Determines whether update does anything or not
    QPixmap *nullimage;   //If an image does not exist, use this instead
    imgPage* blankPage;   //If the page does not exist, use this instead

public:
    Spriteflow();
    Spriteflow(int*, int*);        //Constructor to auto inherit position
    void update();                 //Call per frame to update image
    void addChangeFrame(int,int);  //When A frame is reached go to B frame
    void addChangeFrame(QList<ChangeFrame>);// Given a list of changeframes, adds them all
    void editChangeFrame(int,int,int); //Changes a ChangeFrame at index a0
    void removeChangeFrame(int);   //Removes ChangeFrame at index a0
    void addImgFrame(QString,int,QPixmap); //When A frame is reached, set img to arg
    void addImgFrame(QString,int,QString); //When A frame is reached, set img by name
    void addImgFrame(QString,int,int);     //When A frame is reached, set list img index
    void addImgFrame(QList<ImgFrame>);     //Given a list of ImgFrames, adds them all
    void editImgFrame(int,QString,int,QString);//Changes an Imgframe at index a0
    void removeImgFrame(int);      //Removes ImgFrame at index a0
    void addImage(QString,QPixmap);//Adds img into saved list
    void addImage(QList<imgPage*>);//Given a list of images, adds them all
    void removeImage(int);         //Deletes Image at a0;
    void play(QPixmap);            //Sets the current image to arg
    void play(int);                //Sets the current image to arg by index
    void play(QString);            //Sets the current image to name id
    void play();                   //Sets playing to true
    void stop();                   //Sets playing to false
    void inheritPos(int*, int*);   //Inherits cordinates from parent
    bool sendToScene();          //Draws the current image in the EZScene global variable
    bool sendToScene(QGraphicsScene*);//Draws the current image in arg scene
    int isChangeFrame();           //Returns the label associated if changeframe is known
    int isAnim();                  //Returns the imageframe list index if ID matches
    void setImgFrame(int);   //Sets the image to the stored imgframe based on IDD arg
    void ani(int);           //Sets the image to the stored imgframe based in index arg
    QPixmap** findImg(QString);     //Returns the image associated with a name
    QList<ChangeFrame>* getChangeFrame();//Returns the changeframe list
    QList<ImgFrame>* getImageFrame();//    Returns the imageframe list
    QList<imgPage*>* getImagePages();//    Returns the imageframe list
    void decrement(int = 1);        //Decreases the ID by arg amount, or 1 by default
    void increment(int = 1);        //Increases the ID by arg amoubt, or 1 by default

    QString getSpriteName(QString);//finds the sprite name associated with the imgframe
    float getID();//Getter for current ID
    float getMax();//Getter for the maximum sprite ID
    bool getPlaying() const;//Getter for if the sprite is playing
    void setID(float newID);//Setter for the current ID
    void setMax(float newMax);//Setter for the maximum sprite ID
    QList<QString> openFile(QString);
    void extracted(QList<QString> &cflist, QList<ChangeFrame> &RV);
    QList<ChangeFrame> strToChangeList(QList<QString>);
    QList<ImgFrame> strToImgFrameList(QList<QString>,QList<QString>*);
    QList<imgPage*> strToimgPageList(QList<QByteArray>);
    QByteArray substr(QByteArray, QString, QString);
    QList<QString> substrlist(QByteArray, QString, QString);
    QList<QByteArray> subarrlist(QByteArray, QString, QString);


public slots:
    void updateparams();// Updates parameters from parent
};

bool setScene(QGraphicsScene*);//Sets Global Scene to arg. Returns 0 if arg is null

#endif // SPRITEFLOW_H
