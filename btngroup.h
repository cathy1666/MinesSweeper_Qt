#ifndef BTNGROUP_H
#define BTNGROUP_H

#include <QPushButton>
#include <QMouseEvent>
#include <QTextCodec>
#include <QPalette>
#include <QColor>
#include <QColorGroup>

typedef bool (*FUNCPTR)(void);

class BtnGroup : public QPushButton
{
    Q_OBJECT

public:
    FUNCPTR isGameOver;
    FUNCPTR isMarkedFull;
    BtnGroup(QWidget *parent = 0);
    ~BtnGroup();

    void setMine() { gmine = true; }
    void setOpened();
    void addCount() { gcount++; }
    bool getMine() const { return gmine; }
    int  getCount() const { return gcount; }
    bool getOpened() const { return gopened; }
    bool getMarked() const { return gmarked; }
    void initMine();
signals:
    void press();

protected:
    void mousePressEvent(QMouseEvent *e);

private:
    int gcount;
    bool gmine;
    bool gopened;
    bool gmarked;

    void setColor();
};

#endif // BTNGROUP_H
