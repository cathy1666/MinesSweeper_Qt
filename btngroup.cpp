#include "btngroup.h"
#include <stdio.h>

#define IMG_FLAG "flag.gif"
#define IMG_BOOM "boom.gif"

BtnGroup::BtnGroup(QWidget *parent)
{
    gcount = 0;
    gmine = false;
    gopened = false;
    gmarked = false;
}
BtnGroup::~BtnGroup()
{
}
// initialize each block
void BtnGroup::initMine()
{
    gcount = 0;
    gmine = false;
    gopened = false;
    gmarked = false;
    this->setIcon(QIcon(""));
    this->setText(QString(""));
    this->setStyleSheet("QPushButton{border: 1px solid #8f8f91;"
                          "background-color: #C0C0C0;}");
}

// enter mousePressEvent function if mouse press on a button.
void BtnGroup::mousePressEvent(QMouseEvent *e)
{
    //mouse left-click
    if(e->button() & Qt::LeftButton && !gopened && !gmarked)
    {
        if(!isGameOver())
            emit press(); //if not game over, emit "press" signal
    }
    //mouse right-click
    else if(e->button() & Qt::RightButton && !gopened)
    {
        if((gmarked && isMarkedFull()) || (!isMarkedFull() && !isGameOver()))
        {
            this->setIcon((gmarked)? QIcon(""):QIcon(IMG_FLAG));
            gmarked ^= true;
            emit clicked(gmarked);// emit "clicked" signal (true)
        }
    }
}

void BtnGroup::setOpened()
{
    if(!gopened && !gmarked)
    {
        gopened = true;
        this->setColor();
    }
}

void BtnGroup::setColor()
{
    QString head, color, tail;
    //set button border color
    head.sprintf("QPushButton{border: 1px solid #7f7f71;");
    //set button background color
    tail.sprintf("background-color: #3f3f3f}");

    if (gmine)//if the pressed block with a mine, then set a BOOM icon.
    {
        this->setIcon(QIcon(IMG_BOOM));
        //this->setText("*");
        //color.sprintf("color:red;");
    }
    else if(gcount) //if there are mines around, show mine count and text color.
    {
        QString s;
        s.sprintf("%d", gcount);//convert int to String to set button text
        this->setText(s);
        switch(gcount)
        {
        case 1: color.sprintf("color:white;");   break;
        case 2: color.sprintf("color:lime;");    break;
        case 3: color.sprintf("color:yellow;");  break;
        case 4: color.sprintf("color:#009ACD;"); break;
        case 5: color.sprintf("color:#CD0000;"); break;
        case 6: color.sprintf("color:#BDB76B;"); break;
        case 7: color.sprintf("color:orange;");  break;
        case 8: color.sprintf("color:#4EEE94;"); break;
        default:break;
        }
    }
    this->setStyleSheet(head+color+tail);
}

