#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include "btngroup.h"

class QLabel;
class QPushButton;
class QSignalMapper;

#define WIDTH   10
#define HEIGHT  10
#define MINES   20

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void gameReset();

    static bool isGameOver(void);
    static bool isMarkedFull(void);

protected:
    void timerEvent(QTimerEvent *t);

private slots:
    void newGame();
    void open(int id);
    void changeCount(bool marked);

private:
    BtnGroup *map[WIDTH*HEIGHT];

    QLabel *mine_label;
    QLabel *time_label;
    QPushButton *reset_button;//a button for reseting game

    int opened_count;//the opened blocks

    int game_time;//ticking on game start
    int timer_idx;//the ID of timer

    QLabel *mines;
    QSignalMapper *signalMapper;

    void gameOver(const bool won);
    void showAllMines(const int index);
    void openBlock(int x, int y);
};

#endif // MAINWINDOW_H
