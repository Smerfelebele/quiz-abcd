#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGroupBox>
#include <QFile>
#include <QTime>
#include <QTextStream>
#include <QPushButton>
#include <QRadioButton>
#include <QGridLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void handleCheckButton();
    void handleNextButton();
    void handleRestartButton();
private:
    Ui::MainWindow *ui;
    QGroupBox* answers();
    //QGroupBox* createSecondExclusiveGroup();
    QGridLayout *grid;
    QGroupBox* currentQuestion;
    std::vector <int> occurredQuestionsVec;
    bool checkAnswers();
    int points;

    void openFileAndAddAnswers();

    struct QA{
        QString mainQuestion;
        QString answers[4];
        QString correctAnswer;
    };
    QA A;

    QPushButton* check;
    QPushButton* next;
    QPushButton* restart;

    QRadioButton* answerA;
    QRadioButton* answerB;
    QRadioButton* answerC;
    QRadioButton* answerD;


};
#endif // MAINWINDOW_H
