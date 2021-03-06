#ifndef GROUPITEMWIDGET_H
#define GROUPITEMWIDGET_H

#include <QWidget>


#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class HeadIcon;
class QMenu;
QT_END_NAMESPACE

class GroupItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GroupItemWidget(QWidget *parent = 0);
    void setGroupInfo(const QString &grouid, const QString &groupname);
    void setImage(const QString& url);

    QString getGroupName();
    QString getGroupId();
    QString getImagePath();

    void setGroup(bool flag);
    void hideExit();
signals:
    void updateListWidget();
    void exitGroup();
public slots:

private slots:

protected:
    void resizeEvent(QResizeEvent *);
    void contextMenuEvent(QContextMenuEvent *event);
    void listWidgetMenuTriggered();

private:
    void init();

    HeadIcon *m_headicon;               //头像
    QLabel *m_nickname;                 //昵称+备注

    QString groupname;
    QString groupid;
    QString imagePath;

    QMenu *groupMenu;

    QAction *exitMsg;

    bool isGroupItem;                   //判断是否是群组中ITEM
};


#endif // GROUPITEMWIDGET_H
