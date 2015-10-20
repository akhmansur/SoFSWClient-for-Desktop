#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->gogogo,SIGNAL(clicked()),this,SLOT(Start()));
    connect(ui->stop,SIGNAL(clicked()),this,SLOT(Stop()));
    connect(timer, SIGNAL(timeout()), this, SLOT(drawPlot()));
    connect(timerAlarm,SIGNAL(timeout()),this,SLOT(Alarm()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(callTech()));
    for(int i=0;i<6;i++)
    {
        fl[i]=false;
        stat[i]=0;
    }
    for(int i=0;i<3;i++)
        x[i]=30;
    for(int i=6;i<9;i++)
        stat[i]=0;
    ui->temp2Kas1->setText("ЭКЛЗ исправна");
    ui->temp2Kas2->setText("ЭКЛЗ исправна");
    ui->temp2Kas3->setText("ЭКЛЗ исправна");
}

void MainWindow::drawPlot(){
    ticks<<qrand()% (x[0]-(x[0]-10)+1) + (x[0]-10);
    y0<<(time.currentTime().hour()*3600+time.currentTime().minute()*60+time.currentTime().second());
    ui->tempKas1->setText("T = "+QString::number(ticks.last()));
    ui->widget->graph(0)->setData(y0,ticks);
    ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->widget->rescaleAxes();
    ui->widget->yAxis->setRange(0,100);
    ui->widget->replot();
    ticks1<<qrand()% (x[1]-(x[1]-10)+1) + (x[1]-10);
    y1<<(time.currentTime().hour()*3600+time.currentTime().minute()*60+time.currentTime().second());
    ui->tempKas2->setText("T = "+QString::number(ticks1.last()));
    ui->widget_2->graph(0)->setData(y1,ticks1);
    ui->widget_2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->widget_2->rescaleAxes();
    ui->widget_2->yAxis->setRange(0,100);
    ui->widget_2->replot();
    ticks2<<qrand()% (x[2]-(x[2]-10)+1) + (x[2]-10);
    ui->tempKas3->setText("T = "+QString::number(ticks2.last()));
    y2<<(time.currentTime().hour()*3600+time.currentTime().minute()*60+time.currentTime().second());
    ui->widget_3->graph(0)->setData(y2,ticks2);
    ui->widget_3->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->widget_3->rescaleAxes();
    ui->widget_3->yAxis->setRange(0,100);
    ui->widget_3->replot();
    timer->start(1500);

}
void MainWindow::setDefaultXs(){
    for(int i=0;i<3;i++)
        x[i]=30;
    ui->temp2Kas1->setText("ЭКЛЗ исправна");
    ui->temp2Kas2->setText("ЭКЛЗ исправна");
    ui->temp2Kas3->setText("ЭКЛЗ исправна");
}
void MainWindow::setHighXs(int i){
    x[i]=100;
}

void MainWindow::Alarm(){
    int j=qrand()%6;
    stat[j]+=1;
    if(j>=0 & j<3){
        stat[7]+=1;
        if(j==0) ui->srTempKas1->setText(QString::number(stat[0]));
        if(j==1) ui->srTempKas2->setText(QString::number(stat[1]));
        if(j==2) ui->srTempKas3->setText(QString::number(stat[2]));
        setHighXs(j);
        ui->listWidget->addItem(time.currentTime().toString() + " Перегрев термоголовки принтера в кассе №"+QString::number(j+1));
        QMessageBox::information(0, "Внимание!!!", time.currentTime().toString() + " Перегрев термоголовки принтера в кассе №"+QString::number(j+1));
    }
    if(j==3){
        stat[8]+=1;
        ui->srEklzKas1->setText(QString::number(stat[3]));
        ui->temp2Kas1->setText("Авария ЭКЛЗ!");
        ui->listWidget->addItem(time.currentTime().toString() + " Авария ЭКЛЗ в кассе № 1");
        QMessageBox::information(0, "Внимание!!!", time.currentTime().toString() + " Авария ЭКЛЗ в кассе № 1");
    }
    else{
        if(j==4){
        stat[8]+=1;
        ui->srEklzKas2->setText(QString::number(stat[4]));
        ui->temp2Kas2->setText("Авария ЭКЛЗ!");
        ui->listWidget->addItem(time.currentTime().toString() + " Авария ЭКЛЗ в кассе № 2");
        QMessageBox::information(0, "Внимание!!!", time.currentTime().toString() + " Авария ЭКЛЗ в кассе № 2");
        }
        if(j==5)
        {
            stat[8]+=1;
            ui->srEklzKas3->setText(QString::number(stat[5]));
            ui->temp2Kas3->setText("Авария ЭКЛЗ!");
            ui->listWidget->addItem(time.currentTime().toString() + " Авария ЭКЛЗ в кассе № 3");
            QMessageBox::information(0, "Внимание!!!", time.currentTime().toString() + " Авария ЭКЛЗ в кассе № 3");
        }
    }
    ui->srEklzAll->setText(QString::number(stat[8]));
    ui->srTempAll->setText(QString::number(stat[7]));
    timerAlarm->start(qrand()%(30000-9000+1000)+9000);
}

void MainWindow::callTech(){
    ui->listWidget->addItem(time.currentTime().toString()+" Заявка в СПП подана. Вызван техник.");
    stat[6]+=1;
    ui->kolPod->setText(QString::number(stat[6]));
    setDefaultXs();
}

void MainWindow::Start(){

    timer->start(3000);
    timerAlarm->start(qrand()%(30000-9000+1000)+9000);
    time=time.currentTime();
    ui->widget->addGraph();
    ui->widget->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    ui->widget->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    ui->widget->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->widget->xAxis->setDateTimeFormat("mm:ss");
    ui->widget->yAxis->setRange(0,80);
    ui->widget_2->addGraph();
    ui->widget_2->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    ui->widget_2->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    ui->widget_2->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->widget_2->xAxis->setDateTimeFormat("mm:ss");
    ui->widget_2->yAxis->setRange(0,80);
    ui->widget_3->addGraph();
    ui->widget_3->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    ui->widget_3->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    ui->widget_3->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->widget_3->xAxis->setDateTimeFormat("mm:ss");
    ui->widget_3->yAxis->setRange(0,80);
    //start();
}

void MainWindow::Stop(){
    timer->stop();
    timerAlarm->stop();
}

MainWindow::~MainWindow()
{
    delete ui;
}
