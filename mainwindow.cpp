#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QRadioButton>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QButtonGroup>

#define NUMBER_OF_QUESTIONS 22



MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    //, ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    qsrand(static_cast<quint64>(QTime::currentTime().msecsSinceStartOfDay()));
    openFileAndAddAnswers();
    grid = new QGridLayout;
    currentQuestion = answers();
    grid->addWidget(currentQuestion,0,0);
    restart = new QPushButton("Restart",this);
    restart->setFixedSize(80,50);
    grid->addWidget(restart, 0, 1);
    connect(restart, SIGNAL (clicked()),this, SLOT (handleRestartButton()));
    //grid->addWidget(createSecondExclusiveGroup(), 1, 0);
    setWindowTitle(tr("PST"));
    setLayout(grid);
    setFixedSize(900,200);
    points = 0;
    //connect(next, SIGNAL (released()),this, SLOT (handleNextButton()));
    //resize(480,320);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QGroupBox *MainWindow::answers()
{

 QGroupBox* groupBox = new QGroupBox(tr("Odpowiedzi"));
 QLabel* mainQuestion = new QLabel(A.mainQuestion);
 //QLabel* ansA = new QLabel();
 //ansA->setText(A.answers[0]);
 //ansA->setWordWrap(true);
 answerA = new QRadioButton(A.answers[0]);
 //answerA = new QRadioButton();
 answerB = new QRadioButton(A.answers[1]);
 answerC = new QRadioButton(A.answers[2]);
 answerD = new QRadioButton(A.answers[3]);
 QVBoxLayout* vbox = new QVBoxLayout;
 vbox->addWidget(mainQuestion,Qt::AlignTop);
 vbox->addWidget(answerA);
 vbox->addWidget(answerB);
 vbox->addWidget(answerC);
 vbox->addWidget(answerD);
 vbox->addStretch(1);
 check = new QPushButton("Check",this);
 next = new QPushButton("Next",this);
 connect(next, SIGNAL (released()),this, SLOT (handleNextButton()));
 connect(check, SIGNAL (released()),this, SLOT (handleCheckButton()));

 QHBoxLayout* buttonHBox = new QHBoxLayout;

 buttonHBox->addWidget(check);
 buttonHBox->addWidget(next);
 vbox->addLayout(buttonHBox);

 groupBox->setLayout(vbox);
 return groupBox;
}


void MainWindow::openFileAndAddAnswers()
{
    QFile inputFile("C:/Users/komp/Documents/pytaniaPST2/test.txt");
    QTextStream in(&inputFile);
    auto  randomVal = qrand() % NUMBER_OF_QUESTIONS ;
    while(!occurredQuestionsVec.empty())
    {
       bool check = std::find(std::begin(occurredQuestionsVec), std::end(occurredQuestionsVec),randomVal) != std::end(occurredQuestionsVec);
       if(check)
       {
           randomVal = qrand()% NUMBER_OF_QUESTIONS;
       }
       else
           break;
    }
    occurredQuestionsVec.push_back(randomVal);
    if(occurredQuestionsVec.size()==NUMBER_OF_QUESTIONS)
    {
        occurredQuestionsVec.clear();
        QMessageBox msgBox;

        msgBox.setText(tr("Przeszedłeś przez wszystkie pytania, Twoj wynik to:")+QString::number(points));
        points = 0;
        msgBox.exec();

    }
    if(randomVal!=0)
        randomVal*=7;
    int line = 0;
    bool inQuestion = false;

    if(inputFile.open(QIODevice::ReadOnly))
    {
        int i=0;
        while(!in.atEnd())
        {
            if(line == randomVal+6)
            {
                inQuestion = false;
                break;
            }
            else if(inQuestion){
                if(i<4){
                    A.answers[i] = in.readLine();
                    i++;
                }
                else
                {

                    A.correctAnswer = in.readLine();
                }
            }
            else if(line == randomVal)
            {
                //qDebug() << "Main question:" << in.readLine();
                A.mainQuestion = in.readLine();
                inQuestion = true;
            }
            else
            {
                in.readLine();
            }
            line++;
        }
    }
}

void MainWindow::handleCheckButton()
{
QMessageBox msgBox;
msgBox.setFixedSize(100,75);
   if(checkAnswers())
       msgBox.setText("Correct!");
   else
       msgBox.setText("Incorrect");

   msgBox.exec();
}

void MainWindow::handleNextButton()
{
       if(checkAnswers())
           points++;

       delete currentQuestion;
       openFileAndAddAnswers();
       currentQuestion = answers();
       grid->addWidget(currentQuestion,0,0);
}

void MainWindow::handleRestartButton()
{
    points =0;
    occurredQuestionsVec.clear();
}

bool MainWindow::checkAnswers()
{
    QButtonGroup group;

    QList <QRadioButton *> allButtons = currentQuestion->findChildren<QRadioButton *>();

   for(int i=0;i<allButtons.size();++i)
   {
       group.addButton(allButtons[i],i);
   }

   if(A.correctAnswer.toInt() == group.checkedId())
       return true;

   return false;
}
