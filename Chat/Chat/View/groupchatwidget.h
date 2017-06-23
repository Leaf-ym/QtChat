#ifndef GROUPCHATWIDGET_H
#define GROUPCHATWIDGET_H

#include "basicwidget.h"
#include "BasicControls/chatinput.h"

class HeadIcon;
class QPushButton;
class QListWidget;
class GroupMemberListWidget;

class GroupChatWidget : public BasicWidget
{
    Q_OBJECT
public:
    GroupChatWidget(QWidget *parent =Q_NULLPTR);
    void setUserName(const QString &username);
    void setUserid(const QString &userid);
    void setIcon(const QString &path);

signals:
    void updateMessage();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void setMessage(const QString&);

public slots:
    void showMessage(const QString &msg, const QString &color, const QString &size, const QString &family);

private:


    void init();

    HeadIcon *headIcon;
    QLabel *lb_username;

    QPushButton *btn_chat_settings;
    QPushButton *btn_chat_close;
    QPushButton *btn_chat_min;
    QPushButton *btn_chat_max;

    QTextEdit *textedit;

    ChatInput *chatinput;

    QString userid;

    GroupMemberListWidget *listwidget;

    bool hide_list = false;
    QPushButton *btn_hide_list;
};
#endif // GROUPCHATWIDGET_H
