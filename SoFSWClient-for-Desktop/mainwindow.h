#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QThread>
#include <QKeyEvent>
#include <QTimer>
#include <QMenu>
#include <QMenuBar>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QMap<QString,QThread*> sessionMap;
    QNetworkReply *reply;
    void RespPars(QByteArray str);
    void sendPost(QString comm);
    QString reqOldMes;
    QMap<QString, QString>* roomsMap;
    QStringList mapC;
    QStringList mapP;
    QStringList roomList;
    QMenu* menu;
    QMenuBar menubarr;
    //void AddToChatList(QString from);
    int chatminid = 2147483647;
    bool eventFilter(QObject* obj, QEvent *event);
    ~MainWindow();

private slots:
    void replyFinished(QNetworkReply *reply);
    void resultIsHere(QByteArray qstr);
    void on_pushButton_2_clicked();
    void errorString(QString str);
    void on_pushButton_4_clicked();
    void timeOut();
    void chatClicked(QUrl url);
    void on_sendToGame_clicked();
    void sendToChat_clicked();
    void on_pushButton_clicked();
    void changeRoom(QString i);
    void on_pushButton_3_clicked();
    void actionMenuClicked1();
    void actionMenuClicked2();
    void actionMenuClicked3();
    void actionMenuClicked4();
    void actionMenuClicked5();
    void actionMenuClicked6();
    void actionMenuClicked7();
    void on_lineEdit_2_returnPressed();

private:
    QPixmap* backgr;
    QPixmap* backgrr;
    QTimer* timer;
    bool inFight;
    QString getMacAddress();
    QString SpecialXmlEscapeEnc(QString input);
    void AddToChat(QString from, QString to, QString message, QString dtime, bool priv, bool totop, int tid);
    void AddChatRoomB(QString chnum, QString chname, QString des, QString incount);
    void AddButtonG(QString kay,QString ctxt);
    void AddGText(QString gtext);
    void SetPname(QString nm);
    void SetPlev(QString de, QString lv);
    void SetPHP(QString hpdes, QString hp, QString hpmax);
    void SetPSP(QString spdes, QString sp, QString spmax);
    void SetPPT(QString ptdes, QString pt, QString ptmax);
    void SetAtten(QString p);
    void addLog(QString logStr);
    bool flag;
    QString name;
    int tick;
    QStringList commList;
    void AddChatRoomD(QString chnm, QString chdes, QString chincount);
    void AddButC(QString kay, QString txt);
    void UpdateMap(QString x, QString y, QString code);
    Ui::MainWindow *ui;
    QString goText;
    QString* deviceId;
signals:

};

#endif // MAINWINDOW_H
