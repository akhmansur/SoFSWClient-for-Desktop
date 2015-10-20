#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include "qcustomplot.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QTime time;
    int j;
    int *x=new int [2];
    bool *fl=new bool[5];
    int *stat=new int[8];
    QTimer *timer = new QTimer(this);
    QTimer* timerAlarm=new QTimer(this);
    QVector<double> ticks, y0,ticks1,y1,ticks2,y2;
    bool startStopFL;
    void start();
    void setDefaultXs();
    void setHighXs(int i);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
public slots:
    void Start();
    void Stop();
    void drawPlot();
    void callTech();
    void Alarm();
};

#endif // MAINWINDOW_H
