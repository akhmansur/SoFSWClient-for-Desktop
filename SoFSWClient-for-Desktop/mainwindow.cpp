#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "networkworker.h"
#include <QXmlSimpleReader>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrlQuery>
#include <QThread>
#include <QDomDocument>
#include <QDomElement>
#include <QKeyEvent>
#include <QTimer>
#include <QPainter>
#include <QUuid>
#include <QNetworkInterface>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->installEventFilter(this);
    menu=new QMenu;
    this->setFocus();
    timer=new QTimer(this);
    timer->setInterval(1100);
    timer->start();
    connect(timer,SIGNAL(timeout()),this,SLOT(timeOut()));
    flag=false;
    tick=0;
    menu->setTitle("Команды");
    menu->addAction("&Персонаж",this,SLOT(actionMenuClicked1()));
    menu->addAction("&Вещи",this,SLOT(actionMenuClicked2()));
    menu->addAction("&Осмотреться",this,SLOT(actionMenuClicked3()));
    menu->addAction("&Информация",this,SLOT(actionMenuClicked4()));
    menu->addAction("&Магазин",this,SLOT(actionMenuClicked5()));
    menu->addAction("&Акции",this,SLOT(actionMenuClicked6()));
    menu->addAction("&Настройки",this,SLOT(actionMenuClicked7()));
    ui->menuBar->addMenu(menu);
    ui->menuBar->setStyleSheet("background-color:#ffffff;");
    roomsMap=new QMap<QString, QString>;
    commList.append("chatmess !chroom? list");
    commList.append("chatmess !chroom? descr");
    commList.append("getmappoints");
    commList.append("chatmess !history");
    backgrr=new QPixmap;
    backgr=new QPixmap;
    backgrr->load(":/images/background_l.png");
    *backgr=backgrr->scaled(ui->map->size());
    connect(ui->chat,SIGNAL(anchorClicked(QUrl)),this,SLOT(chatClicked(QUrl)));
    connect(ui->lineEdit,SIGNAL(returnPressed()),this,SLOT(on_sendToGame_clicked()));
    connect(ui->lineEdit_3,SIGNAL(returnPressed()),this,SLOT(sendToChat_clicked()));
    connect(ui->rooms,SIGNAL(activated(QString)),this,SLOT(changeRoom(QString)));
    deviceId=new QString;
    *deviceId=getMacAddress();
    qDebug()<<*deviceId;
}

QString MainWindow::getMacAddress()
{
    foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
    {
        if (!(interface.flags() & QNetworkInterface::IsLoopBack))
            return interface.hardwareAddress();
        QString text = interface.hardwareAddress();

    }
    return QString();
}

void MainWindow::chatClicked(QUrl url){
    ui->lineEdit_3->setText(ui->lineEdit_3->text()+url.toString()+", ");
}

bool MainWindow::eventFilter(QObject* obj, QEvent *event)
{
    if (obj == ui->lineEdit)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Up)
            {
                ui->lineEdit->setText("");
                ui->lineEdit->setText("n");
                 return true;
            }
            if(keyEvent->key() == Qt::Key_Down)
            {
                ui->lineEdit->setText("");
                ui->lineEdit->setText("s");
                return true;
            }
            if(keyEvent->key() == Qt::Key_Left)
            {
                ui->lineEdit->setText("");
                ui->lineEdit->setText("w");
                return true;
            }
            if(keyEvent->key() == Qt::Key_Right)
            {
                ui->lineEdit->setText("");
                ui->lineEdit->setText("e");
                return true;
            }
        }
        return false;
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "ERROR: " << reply->errorString();
    }
    else
    {
        QByteArray responseData = reply->readAll();
        QString qstr(responseData);
        qstr=qstr.fromUtf8(responseData);
        //qDebug() << "SUCCESS: " << qstr;
    }
}

void MainWindow::timeOut(){
    if(commList.count()==0)
    {
        if(tick>4 & !inFight){
            sendPost("0");
            addLog("0");
            tick=0;
        } else {
            tick++;
        }
    }else{
        tick=0;
        sendPost(commList.first());
        addLog(commList.first());
        commList.removeFirst();
    }
}

void MainWindow::sendPost(QString comm)
{
    QMap<QString,QString> data;
    //data.insert("i","ffffffff-9404-f152-ae82-42366efe6b62");
    data.insert("i",*deviceId);
    qDebug()<<*deviceId;
    data.insert("j",comm);
    data.insert("v","a.1.0.7.7");
    NetworkWorker* netW=new NetworkWorker();
    QThread* thread=new QThread;
    netW->moveToThread(thread);
    netW->setLabelData(data);
    connect(netW, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), netW, SLOT(postRequest()));
    connect(netW, SIGNAL(finished()), thread, SLOT(quit()));
    connect(netW, SIGNAL(sendResult(QByteArray)), this, SLOT(resultIsHere(QByteArray)));
    connect(netW, SIGNAL(finished()), netW, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void MainWindow::resultIsHere(QByteArray qstr){
    RespPars(qstr);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete backgr;
    delete deviceId;
    delete menu;
    delete roomsMap;
    delete timer;
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->label_6->clear();
}
void MainWindow::errorString(QString str){
    qDebug()<<"error!!!";
}

void MainWindow::RespPars(QByteArray str){
    QDomDocument domDoc;
    QString string(str);
    domDoc.setContent(str);
    QDomNodeList nnodes = domDoc.childNodes();
    QDomNodeList nodes=nnodes.item(1).childNodes();
    /*if(chatList){
        ui->chatList->setHtml("");
    }*/

    for (int i = 0; i < nodes.count(); i++) {
        QDomElement element = nodes.item(i).toElement();
            if(!QString::compare(element.tagName(),"mess")){
                if(!QString::compare(element.attribute("type"),"game")) {
                        addLog("--->");
                        ui->label->setText("");
                        QDomNodeList mnodes = nodes.item(i).childNodes();
                        for (int j = 0; j < mnodes.count(); j++) {
                            QDomElement melement = mnodes.item(j).toElement();
                            if(!QString::compare(melement.tagName(),"text")) {
                                    //cmdFr.addLog(melement.text());
                                     AddGText(melement.text());
                                     QDomNodeList lnodes = mnodes.item(j).childNodes();
                                     for (int l = 0; l < lnodes.count(); l++) {
                                        QDomElement gelement = lnodes.item(l).toElement();
                                        /*switch (gelement.tagName()) {
                                            case "lynk":
                                                //AddLnkButt(gelement.attribute("text"), gelement.attribute("a"));
                                                break;
                                        }*/
                                     }
                                 }
                                 if(!QString::compare(melement.tagName(),"comm")){
                                    QString kay = "";
                                    QString ctxt = "";
                                    QDomNodeList gnodes = mnodes.item(j).childNodes();
                                    for (int l = 0; l < gnodes.count(); l++) {
                                        QDomElement gelement = gnodes.item(l).toElement();
                                            if(!QString::compare(gelement.tagName(),"kay")) {
                                                kay = gelement.text();
                                            }
                                            if(!QString::compare(gelement.tagName(),"ctxt")){
                                                ctxt = gelement.text();
                                            }
                                     }
                                     AddButtonG(kay, ctxt);
                                     addLog(kay + "- " + ctxt);
                                     //cont = true;
                                 }
                                  if(!QString::compare(melement.tagName(),"point")){
                                    UpdateMap(melement.attribute("x"), melement.attribute("y"), melement.attribute("code"));
                                    }
                             }
                         }
                if(!QString::compare(element.attribute("type"),"chat")){
                    //cont = true;
                    QString from = "";
                    QString to = "";
                    QString mtext = "";
                    QString dtime = "";
                    int tid = 1;
                    bool totop = false;
                    QDomNodeList cmnodes = nodes.item(i).childNodes();
                    for (int j = 0; j < cmnodes.count(); j++) {
                        QDomElement gelement = cmnodes.item(j).toElement();
                            if(!QString::compare(gelement.tagName(),"from")) {
                                from = gelement.text();
                            }
                            if(!QString::compare(gelement.tagName(),"to")){
                                to = gelement.text();
                            }
                            if(!QString::compare(gelement.tagName(),"mtext")){
                                mtext = SpecialXmlEscapeEnc(gelement.text());
                            }
                            if(!QString::compare(gelement.tagName(),"dtime")){
                                dtime = gelement.text();
                            }
                            if(!QString::compare(gelement.tagName(),"mid")){
                                QString tss = gelement.text();
                                tid = tss.toInt();
                            }
                    if((from!="")&(mtext!="")) {
                            if(!QString::compare(element.attribute("totop"),"1")) {
                                totop = true;
                            }
                            else{
                                totop = false;
                            }
                            if(!QString::compare(element.attribute("room"),"private")) {
                                AddToChat(from, to, mtext, dtime, true, totop, tid);
                            }
                            else{
                                AddToChat(from, to, mtext, dtime, false, totop, tid);
                            }
                    }
                }
                }
                if(!QString::compare(element.attribute("type"),"chatrooms")){
                    ui->chat->clear();
                    roomsMap->clear();
                    ui->rooms->clear();
                    roomList.clear();
                    QDomNodeList crnodes = nodes.item(i).childNodes();
                    for (int j = 0; j < crnodes.count(); j++) {
                        QString chnum = "";
                        QString chname = "";
                        QString des = "";
                        QString incount = "";
                        QDomNodeList lnodes = crnodes.item(j).childNodes();
                        for (int l = 0; l < lnodes.count(); l++) {
                            QDomElement gelement = lnodes.item(l).toElement();
                                if(!QString::compare(gelement.tagName(),"num")){
                                    chnum = gelement.text();
                                }
                                if(!QString::compare(gelement.tagName(),"name")){
                                    chname = gelement.text();
                                }
                                if(!QString::compare(gelement.tagName(),"des")){
                                    des = gelement.text();
                                }
                                if(!QString::compare(gelement.tagName(),"incount")){
                                    incount = gelement.text();
                                }
                             }
                            AddChatRoomB(chnum, chname, des, incount);
                         }

                 }
                 if(!QString::compare(element.attribute("type"),"chatroomdes")){

                    QString chnm = "";
                    QString chdes = "";
                    QString chincount = "";
                    QDomNodeList cnodes = nodes.item(i).childNodes();
                    for (int j = 0; j < cnodes.count(); j++) {
                       QDomElement melement = cnodes.item(j).toElement();
                        if(!QString::compare(melement.tagName(),"name")) {
                            chnm = melement.text();
                        }
                        if(!QString::compare(melement.tagName(),"des")){
                            chdes = melement.text();
                        }
                        if(!QString::compare(melement.tagName(),"incount")){
                            chincount = melement.text();
                        }
                    }
                    AddChatRoomD(chnm, chdes, chincount);
                 }
                 if(!QString::compare(element.attribute("type"),"mappoints")){
                    mapP.clear();
                    mapC.clear();
                    QDomNodeList mpnodes = nodes.item(i).childNodes();
                    for (int j = 0; j < mpnodes.count(); j++) {
                        QDomElement melement = mpnodes.item(j).toElement();
                        if(!QString::compare(melement.tagName(),"mpt")) {
                                mapC.append(melement.attribute("x") + ":" + melement.attribute("y"));
                                mapP.append(melement.attribute("c"));
                        }
                     }
                  }
                  if(!QString::compare(element.attribute("type"),"shop")){
                    addLog("На данный момент данные возможности реализованы только в клиенте для Windows");
                  }
               }
           if(!QString::compare(element.tagName(),"Commands")){
               ui->label->clear();
                QDomNodeList mnodes = nodes.item(i).childNodes();
                for (int j = 0; j < mnodes.count(); j++) {
                    QString kay = "";
                    QString ctxt = "";
                    QDomNodeList gnodes = mnodes.item(j).childNodes();
                    for (int l = 0; l < gnodes.count(); l++) {
                        QDomElement gelement = gnodes.item(l).toElement();
                           if(!QString::compare(gelement.tagName(),"kay")) {
                                kay = gelement.text();
                           }
                           if(!QString::compare(gelement.tagName(),"ctxt")){
                                ctxt = gelement.text();
                           }
                        }
                     AddButC(kay, ctxt);
                  }
           }
              if(!QString::compare(element.tagName(),"Settings")){
                if(flag) {
                    QDomNodeList setnodes = nodes.item(i).childNodes();
                    for (int j = 0; (j < setnodes.count()); j++) {
                    QDomElement setmelement = setnodes.item(j).toElement();
                    /*switch (setmelement.tagName()) {
                        case "Theme":
                            switch (setmelement.text()) {
                                case "Dark":
                                    Utils.isLight=false;
                                    break;
                                case "Light":
                                    Utils.isLight=true;
                                    break;
                            }
                            break;
                        case "push_rdy":
                            switch (setmelement.text()) {
                                case "0":
                                    Utils.toastHpIsAcc=false;
                                    break;
                                case "1":
                                    Utils.toastHpIsAcc=true;
                                    break;
                            }
                            break;
                        case "push_prmes":
                            switch (setmelement.text()) {
                                case "0":
                                    Utils.toastPrMesIsAcc=false;
                                    break;
                                case "1":
                                    Utils.toastPrMesIsAcc=true;
                                    break;
                            }
                            break;
                        case "push_chmes":
                            switch (setmelement.text()) {
                                case "0":
                                    Utils.toastChMesIsAcc=false;
                                    Utils.flag=false;
                                    if(!Utils.isLight){
                                    Utils.changeToTheme(this,Utils.THEME_DARK);
                                    titlestrip.setTextColor(Color.WHITE);
                                    titlestrip.setBackgroundColor(Color.BLACK);
                                    }
                                    break;
                                case "1":
                                    Utils.toastChMesIsAcc=true;
                                    Utils.flag=false;
                                    if(!Utils.isLight){
                                        Utils.changeToTheme(this,Utils.THEME_DARK);
                                        titlestrip.setTextColor(Color.BLACK);
                                        titlestrip.setBackgroundColor(Color.WHITE);
                                    }
                                    break;
                                }
                                break;
                            }*/
                        }
                    }
                }
                if(!QString::compare(element.tagName(),"perdata")){
                    QDomNodeList pnodes = nodes.item(i).childNodes();
                        for (int j = 0; j < pnodes.count(); j++) {
                        QDomElement gelement = pnodes.item(j).toElement();
                            if(!QString::compare(gelement.tagName(),"pname")) {
                                SetPname(gelement.text());
                            }
                            if(!QString::compare(gelement.tagName(),"plev")){
                                SetPlev(gelement.attribute("ldes"), gelement.attribute("lev"));
                            }
                            if(!QString::compare(gelement.tagName(),"php")){
                                SetPHP(gelement.attribute("hpdes"), gelement.attribute("hp"), gelement.attribute("hpmax"));
                            }
                            if(!QString::compare(gelement.tagName(),"psp")){
                                SetPSP(gelement.attribute("spdes"), gelement.attribute("sp"), gelement.attribute("spmax"));
                            }
                            if(!QString::compare(gelement.tagName(),"ppt")){
                                SetPPT(gelement.attribute("ptdes"), gelement.attribute("pt"), gelement.attribute("ptmax"));
                            }
                            if(!QString::compare(gelement.tagName(),"atten")){
                                SetAtten(gelement.attribute("on"));
                            }
                         }
                    }
                if(!QString::compare(element.tagName(),"error")){
                        addLog("ERROR");
                }
            }
        }

QString MainWindow::SpecialXmlEscapeEnc(QString input)
    {
        QString strXmlText = input;

        int p = strXmlText.indexOf(":amp:#");
        while (p > -1) {
                int e = strXmlText.indexOf(";", p);
                QString st = strXmlText.mid(p ,(e+1)-p);
                strXmlText = strXmlText.replace(p,(e+1)-p, "");
                p=strXmlText.indexOf(":amp:#");
            }
        return strXmlText;
    }

void MainWindow::UpdateMap(QString x, QString y, QString code){
    *backgr=backgrr->scaled(ui->map->size());
    QPainter* painter=new QPainter();
    //painter->drawPixmap(0,0, backgr.scaled(ui->map));
    if (mapC.indexOf(x + ":" + y) < 0) {
        mapC.append(x + ":" + y);
        mapP.append(code);
    }
    int cx = x.toInt();
    int cy = y.toInt();
    QString tx;
    QString ty;
    QPixmap image(":/images/s01_l.png");
    int rz = image.height();
    int hmc = (backgr->width() / 2) - 1;
    int vmc = (backgr->height() / 2);
    int hdc = (hmc / rz);
    int vdc = (vmc / rz);
    for (int i = (-1 * hdc - 1); i <= hdc; i++) {
        for (int j = (-1 * vdc); j <= vdc + 1; j++) {
            //qDebug()<<cx+i<<" "<<cy+j;
            tx = QString::number(cx + i);
            ty = QString::number(cy + j);
            int pt = mapC.indexOf(tx + ":" + ty);
            if (pt != -1) {
                    QString str=(mapP.mid(pt,1)).first();
                    //qDebug()<<j;
                    QString path=":/images/"+str+"_l.png";
                    QPixmap pix(path);
                    painter->begin(backgr);
                    painter->drawPixmap(hmc + i * rz, vmc - j * rz, pix);
                    painter->end();
            }
        }
    }
    painter->begin(backgr);
    painter->drawPixmap(hmc, vmc, image);
    painter->end();
    ui->map->setPixmap(*backgr);
}

void MainWindow::AddGText(QString gtext){
    if(gtext.contains("бой. (")){ inFight=true;}else inFight=false;
    ui->label->setText(ui->label->toPlainText()+gtext);
    addLog(ui->label->toPlainText());
}

void MainWindow::AddButtonG(QString kay,QString ctxt){
    ui->label->setText(ui->label->toPlainText()+"\n"+kay+" - "+ctxt+"\n");
}

void MainWindow::addLog(QString logStr){
    ui->label_6->setText(logStr+"\n"+ui->label_6->toPlainText());
}

void MainWindow::AddToChat(QString from, QString to, QString message, QString dtime, bool priv, bool totop, int tid) {
            //LinearLayout ll = (LinearLayout) v.findViewById(R.id.chatContent);
            if (chatminid > tid) chatminid = tid;
            QString shou = "";
            QString smin = "";
            if (!QString::compare(dtime,"none")) {
                int min=QTime::currentTime().minute();
                int hou=QTime::currentTime().hour();
                if (min < 10) {
                    shou = QString::number(hou) + ":0" + QString::number(min);
                } else {
                    shou = QString::number(hou) + ":" + QString::number(min);
                }
            }else{
                shou=dtime;
            }
            if(totop)
            {
                if (priv) {
                    ui->chat->setHtml(shou + " приватно от " + from + ":<br>" + message+ui->chat->toHtml());
                } else {
                    QString prom=shou+ " " +"<a href=\""+from+"\">"+ from + "</a>: " + message;
                    ui->chat->setHtml("<style> a{text-decoration: none;}</style>"+prom+ui->chat->toHtml());
                }
            }else{
                if (priv) {
                    ui->chat->setHtml(ui->chat->toHtml()+shou + " приватно от " + from + ":<br>" + message);
                } else {
                    QString prom=shou+ " " +"<a href=\""+from+"\">"+from + "</a>: " + message;
                    ui->chat->setHtml("<style> a{text-decoration: none;}</style>"+ui->chat->toHtml()+ prom);
                }
            }
}

void MainWindow::AddChatRoomB(QString chnum, QString chname, QString des, QString incount) {
           QString prom="Комната: " + chname + "  {" + incount + "чел.}\n" + des;
           roomsMap->insert(prom,chnum);
           ui->rooms->addItem(prom);
}

void MainWindow::AddChatRoomD(QString chnm, QString chdes, QString chincount) {
            ui->chatRoomName->setText("Комната: " + chnm + "  {" + chincount + "чел.}\n" + chdes);
    }
void MainWindow::AddButC(QString kay, QString txt) {
            if (txt != "") {
                ui->label->setText(ui->label->toPlainText()+kay+" - "+txt+"\n");
            }
}

void MainWindow::SetPname(QString nm) {
        ui->perdata->setText(nm);
        name=nm;
}

void MainWindow::SetPlev(QString de, QString lv) {
            ui->perdata->setText(name+"  " + de + lv);
}

void MainWindow::SetPHP(QString hpdes, QString hp, QString hpmax) {
            ui->hpbar->setText(hpdes+ hp + "/" + hpmax);
            ui->progressBar->setMaximum(100);
            ui->progressBar->setValue((int)(hp.toDouble()/(hpmax.toDouble()/100)));
}

void MainWindow::SetPSP(QString spdes, QString sp, QString spmax) {
            ui->spbar->setText(spdes+sp + "/" + spmax);
            ui->progressBar_2->setMaximum(100);
            ui->progressBar_2->setValue(sp.toDouble()/(spmax.toDouble()/100));
}

void MainWindow::SetPPT(QString ptdes, QString pt, QString ptmax) {
            QString shpt = pt;
            QString shptmax=ptmax;
            int i = ptmax.toInt() - pt.toInt();
            if (pt.length() > 5) {
                if(pt.length()>8){
                    shpt = pt.left(pt.length() - 6) + "m";
                    i=i/1000000;
                    ui->expbar->setText(ptdes+shpt + "/" + QString::number(i)+"m");
                }else{
                    shpt = pt.left(pt.length() - 3) + "k";
                    i=i/1000;
                    ui->expbar->setText(ptdes+shpt + "/" + QString::number(i)+"k");}
            }

            ui->progressBar_3->setValue(pt.toDouble()/(ptmax.toDouble()/100));
}

void MainWindow::SetAtten(QString p) {
            if (p.contains("1")) {
                ui->perdata->setText(ui->perdata->text()+"\nРаспределите характеристики");
            }
}

void MainWindow::on_pushButton_4_clicked()
{
    reqOldMes="chatmess !history " + QString::number(chatminid);
    commList.append(reqOldMes);
    addLog("chatmess !history " + QString::number(chatminid));
}

void MainWindow::on_sendToGame_clicked()
{
    commList.append(ui->lineEdit->text());
    ui->lineEdit->clear();
}

void MainWindow::sendToChat_clicked()
{
    commList.append("chatmess " + ui->lineEdit_3->text());
    addLog("chatmess " + ui->lineEdit_3->text());
    ui->lineEdit_3->clear();
}

void MainWindow::on_pushButton_clicked()
{
    commList.append("getcomms");
    ui->label->clear();
}

void MainWindow::on_pushButton_3_clicked()
{
    commList.append(ui->lineEdit_2->text());
    ui->lineEdit_2->clear();
}

void MainWindow::on_lineEdit_2_returnPressed()
{
    on_pushButton_3_clicked();
    ui->lineEdit_2->clear();
}
void MainWindow::changeRoom(QString i){
    commList.append("chatmess !chroom! "+roomsMap->value(i));
    ui->chat->clear();
}
void MainWindow::actionMenuClicked1(){
        commList.append("01");
}
void MainWindow::actionMenuClicked7(){
        commList.append("SETTINGS");
}
void MainWindow::actionMenuClicked2(){
        commList.append("02");
}
void MainWindow::actionMenuClicked3(){
        commList.append("05");
}
void MainWindow::actionMenuClicked4(){
        commList.append("09");
}
void MainWindow::actionMenuClicked5(){
        commList.append("08");
}
void MainWindow::actionMenuClicked6(){
        commList.append("promo");
}
