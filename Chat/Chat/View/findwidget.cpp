#include "findwidget.h"
#include "validatewidget.h"
#include "BasicControls/headicon.h"
#include "BasicControls/searchlineedit.h"
#include "NetWork/httpconnect.h"
#include "NetWork/connecttoserver.h"
#include <QFile>
#include <QLabel>
#include <QMovie>
#include <QThread>
#include <QPixmap>
#include <QJsonObject>
#include <QJsonDocument>

#include <QPushButton>

FindWidget::FindWidget(QWidget *parent) : BasicWidget(parent)
{
    setAdjustmentSize(false);
    setWidgetTitle("添加好友");
    setFixedSize(600, 500);
    setBackgroundColor(QColor(140, 180, 200));
    init();

    this->setStyleSheet("QLabel{color:white;}");
}


void FindWidget::init()
{
    lineedit = new SearchLineEdit(this);
    lineedit->resize(400, 50);
    lineedit->move(50 , 100);

    btn_searach = new QPushButton("查找", this);
    btn_searach->setFixedSize(100, 50);
    btn_searach->move(460, 100);
    btn_searach->setStyleSheet("QPushButton{border-radius:6px; color:white;}"
                               "QPushButton{border-image:url(:/Resource/mainwidget/blue_normal.png);}"
                               "QPushButton:hover{border-image:url(:/Resource/mainwidget/blue_hover.png);}"
                               "QPushButton:pressed{border-image:url(:/Resource/mainwidget/blue_down.png);}");

    movie = new QMovie(":/Resource/loading.gif");

    label = new QLabel(this);
    label->setFixedSize(600, 470);
    label->move(0, 30);
    label->setStyleSheet("QLabel{background:rgba(255, 255, 255, 200);}");
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label->hide();
    label->setMovie(movie);

    headicon = new HeadIcon(this);
    headicon->setFixedSize(80, 80);
    headicon->move(270, 160);


    userid = new QLabel(this);
    userid->setFixedSize(80, 30);
    userid->move(270, 250);
    userid->setAlignment(Qt::AlignHCenter);

    username = new QLabel(this);
    username->setFixedSize(80, 30);
    username->move(270, 280);
    username->setAlignment(Qt::AlignHCenter);

    add_button = new QPushButton(this);
    add_button->setToolTip("添加好友");
    add_button->resize(32, 32);
    add_button->setStyleSheet("QPushButton{border:0px; color:white;}"
                               "QPushButton{border-image:url(':/Resource/AddUser@2x.png');}"
                               "QPushButton:hover{border-image:url(':/Resource/AddUser_hover@2x.png');}"
                               "QPushButton:pressed{border-image:url(':/Resource/AddUser@2x.png');}");

    add_button->move(330, 275);
    add_button->hide();
    connect(btn_searach, &QPushButton::clicked, this, &FindWidget::searchUser);
    connect(add_button, &QPushButton::clicked, this, &FindWidget::addFriend);

    connect(ConnectToServer::getInstance(), &ConnectToServer::receivedSearchResult, this, &FindWidget::parseUserInfo);

}

void FindWidget::searchUser()
{
    if(label->isHidden()) {
        label->show();
        movie->start();
        ConnectToServer::getInstance()->sendRequestSearchFriend(lineedit->text());
    }
    else
        label->hide();
}

void FindWidget::parseUserInfo(const QByteArray& json)
{
    label->hide();
    movie->stop();
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(json, &error);

    if(!document.isNull())
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();
            QString a = object.value("imagepath").toString();
            QString b = object.value("userid").toString();
            QString c = object.value("username").toString();


            add_button->show();
            username->setText(c);
            userid->setText(b);

            if(a.isEmpty())
            {
                userid->setText("<span style=\"color:red;\">没有该用户</span>");
                headicon->hide();
                add_button->hide();
                return;
            }
            headicon->show();
            add_button->show();
            QString filename = QUrl(a).fileName();
            if(QFile(filename).exists())
            {
                headicon->setPixmap(QPixmap(filename));
            }
            else
            {
                HttpConnect *http = new HttpConnect(this);
                http->loadFileFormUrl(a);

                connect(http, &HttpConnect::loadCompleted, this, [this, filename]()
                {
                   headicon->setPixmap(QPixmap(filename));
                });
            }
        }
    }
    else
        qDebug() << error.errorString();
}

void FindWidget::addFriend()
{
    ValidateWidget *w = new ValidateWidget();
    w->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    w->setAttribute(Qt::WA_ShowModal);
    w->setWindowModality(Qt::ApplicationModal);
    w->show();
    QRect rect = this->geometry();
    int hcenter = width() / 2 + rect.topLeft().x();
    int vcenter = height() / 2 + rect.topLeft().y();
    w->move(hcenter - w->width() / 2, vcenter - w->height() / 2);

    connect(w, &ValidateWidget::info, this, &FindWidget::sendRequest);
}

void FindWidget::sendRequest(const QString& content, const QString &group)
{
    qDebug() << "11";
    ConnectToServer::getInstance()->sendRequestAddFriendAck(userid->text(), group, content);
}
