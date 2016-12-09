#include <QtGui>  //need to include <QtGui> here
#include "mainwindow.h"

#define IMG_RESET "reset.gif"
#define IMG_WIN   "win.gif"
#define IMG_LOSE  "lose.gif"
#define IMG_MINE  "mine.gif"

#define MAX(x,y) (x<=y? y:0)

#define BTNSIZE 30

int marked_count;// the marked blocks
bool game_over;// game over?

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
{
    signalMapper = new QSignalMapper(this);

    mine_label = new QLabel(tr(""));// display remain bomb number
    time_label  = new QLabel(tr(""));// display passing time

    reset_button = new QPushButton(tr(""));
    reset_button->setEnabled(true);// enabled widget handles keyboard and mouse events
    reset_button->setIcon(QIcon(IMG_RESET));
    reset_button->setIconSize(QSize(24,24));
    reset_button->setFixedSize(BTNSIZE, BTNSIZE);// set button size

    connect(reset_button, SIGNAL(pressed()), this, SLOT(newGame()));

    QVBoxLayout *vbox  = new QVBoxLayout;
    QHBoxLayout *hbox  = new QHBoxLayout;
    QHBoxLayout *title = new QHBoxLayout;

    QLabel *bomb = new QLabel;
    QLabel *time = new QLabel;
    bomb->setText("Remain Bomb : ");
    time->setText("Using Time : ");

    title->addWidget(bomb);
    title->addWidget(mine_label);
    title->addSpacing(10);
    title->addWidget(reset_button);
    title->addSpacing(50);
    title->addWidget(time);
    title->addWidget(time_label);

    vbox->addLayout(title);
    vbox->addSpacing(5);

    for(int i=0, index=0; i< HEIGHT; i++)
    {
        int j;
        hbox = new QHBoxLayout;

        for(j=0; j< WIDTH; j++)
        {
            map[index] = new BtnGroup();
            map[index]->isGameOver   = isGameOver;
            map[index]->isMarkedFull = isMarkedFull;
            //set button size
            map[index]->setFixedSize(BTNSIZE, BTNSIZE);

            //set button color
            map[index]->setStyleSheet("QPushButton{border: 1px solid #8f8f91;"
                                      "background-color: #C0C0C0;}");

            connect(map[index], SIGNAL(press()), signalMapper, SLOT(map()));
            signalMapper->setMapping(map[index], index);
            connect(map[index], SIGNAL(clicked(bool)), this, SLOT(changeCount(bool)));

            hbox->addWidget(map[index]);
            hbox->setSpacing(0);
            index++;
        }
        vbox->addLayout(hbox);
        vbox->setSpacing(0);
    }
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(open(int)));
    setLayout(vbox);
    setWindowTitle(tr("My Mines Sweeper"));
}
MainWindow::~MainWindow()
{
}
void MainWindow::open(int id)
{   //openBlock(col, row)
    openBlock(id%WIDTH, id/WIDTH);
}

void MainWindow::changeCount(bool marked)
{
    marked_count += ((marked)? 1:-1);
    QString s;
    s.sprintf("%d", MAX(0, MINES-marked_count));
    mine_label->setText(s);
}

void MainWindow::openBlock(int x, int y)
{
    int index = x + y * WIDTH;// get the index of the map
    BtnGroup *button = map[index];

    //game over or no opened on marked blocks
    if(game_over == true || button->getMarked())
        return;

    //keep opened buttons clicked
    if(button->getOpened() == true)
        return;

    //blocks are opened
    if (!button->getOpened())
        button->setOpened();

    if(button->getMine()) //the block with a mine
    {
        reset_button->setIcon(QIcon(IMG_LOSE));
        showAllMines(index);
        gameOver(false); //BOOM! game over
        return;
    }

    if(button->getCount() > 0) //show number of mines around the block
    {
        if (!button->getOpened())
            button->setOpened();
    }

    opened_count++; //one more block opened

    if(opened_count+MINES == WIDTH*HEIGHT)
    {
        reset_button->setIcon(QIcon(IMG_WIN));
        gameOver(true); //ALL CLEAR! game over
        return;
    }
    //recursive function
    if(button->getCount() == 0) //no mines hide around the block
    {
        // opened the blocks around
        if(y > 0)//not at 1st row
        {
            if(x > 0)//not at 1st col
                openBlock(x-1, y-1);//open upper-left block

            openBlock(x, y-1);//open upper-center block

            if(x < WIDTH-1)//not at last col, then open upper-right block
                openBlock(x+1, y-1);
        }

        if(x > 0)//not at 1st col, then open left block
            openBlock(x-1, y);

        if(x < WIDTH-1)//not at last col, then open right block
            openBlock(x+1, y);

        if(y < HEIGHT-1)//not at last row
        {
            if(x > 0)//not at 1st col, then open bottom-left block
                openBlock(x-1, y+1);

            openBlock(x, y+1);//open bottom-center block

            if(x < WIDTH-1)//not at last col, then open bottom-right block
                openBlock(x+1, y+1);
        }
    }
}
void MainWindow::gameReset()
{
    int size = WIDTH * HEIGHT;
    int i = 0;

    opened_count = 0;
    marked_count = 0;
    game_over = false;
    game_time = 0;
    QString s;
    s.sprintf("%d", MAX(0, MINES-marked_count));
    mine_label->setText(s);

    while(i < MINES)
    {
        int index;
        int row, col;
        index = qrand() % (size + 1);//index range: 0 -(w x h)

        if(map[index]->getMine() == true) //there is a mine
            continue;

        map[index]->setMine();
        row = index / WIDTH;
        col = index % WIDTH;

        //increase count around the blocks
        if(row > 0)//not at 1st block
        {
            if(col > 0)// upper-left , mine not at 1st block
                map[index-WIDTH-1]->addCount();

            map[index-WIDTH]->addCount();//upper-center

            if(col < WIDTH-1)//upper-right, not at last block
                map[index-WIDTH+1]->addCount();
        }

        if(col > 0)//left, not at 1st column
            map[index-1]->addCount();

        if(col < WIDTH-1)//right, not at last column
            map[index+1]->addCount();

        if(row < HEIGHT-1)//not at last row
        {
            if(col > 0) //bottom-left, not at 1st column
                map[index+WIDTH-1]->addCount();

            map[index+WIDTH]->addCount();//bottom-center

            if(col < WIDTH-1) //bottom-right, not at last column
                map[index+WIDTH+1]->addCount();
        }
        i++;
    }
    timer_idx = startTimer(1000);//create and start a timer, and return id to timer_idx
}

//show rest of position of mines
void MainWindow::showAllMines(const int index)
{
    int i = 0, total = WIDTH * HEIGHT;
    do{
        if(map[i]->getMine() == true &&  i != index)
        {
            map[i]->setIcon(QIcon(IMG_MINE));
        }
    }while(i++ < total);
}

void MainWindow::timerEvent(QTimerEvent *t)
{
    if(game_over == true)
    {
        killTimer(timer_idx); //kill and stop timer
        timer_idx = -1;
    }

    game_time++; //increase game time and update it

    QString s;
    s.sprintf("%d", game_time);
    time_label->setText(s);
}

void MainWindow::gameOver(const bool won)
{
    if(game_over == true) return;

    game_over = true;

    if(won == true)//message for winning the game
    {
        QString s;
        s.sprintf("You won! You have cleared the game in %3d seconds.", game_time);
        QMessageBox msgBox;
        msgBox.setText(s);
        msgBox.setPalette( QPalette( Qt::white ) );
        msgBox.exec();
    }
}

void MainWindow::newGame()
{
    time_label->setText(QString("0"));
    reset_button->setIcon(QIcon(IMG_RESET));
    if(!game_over)
    {
        killTimer(timer_idx);
        timer_idx = -1;
    }
    int i = 0, total = WIDTH*HEIGHT;
    do {
        map[i]->initMine();
    }while(++i < total);
    this->gameReset();
}

bool MainWindow::isGameOver()
{
    return game_over;
}

//marked_count is full
bool MainWindow::isMarkedFull()
{
    return (MINES == marked_count);
}

