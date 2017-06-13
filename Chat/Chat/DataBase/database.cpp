#include "database.h"

#include <QDebug>

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("chat.db");
    database.open();
}

DataBase * DataBase::instance = NULL;
QMutex * DataBase::mutex = new QMutex();


DataBase* DataBase::getInstance()
{
    mutex->lock();

    if(instance == NULL)
    {
        instance = new DataBase();
    }

    mutex->unlock();

    return instance;
}

QPair<QString, QString> DataBase::getLocalUserInfo()
{
    QSqlQuery sql_query;
    sql_query.prepare("select * from localuserinfo;");
    sql_query.exec();
    while(sql_query.next())
    {
        int id = sql_query.value(0).toInt();
        QString name = sql_query.value(1).toString();
        QString pw = sql_query.value(2).toString();
        return QPair<QString, QString>(name, pw);
    }
    return QPair<QString, QString>("","");
}

void DataBase::setLoaclUserInfo(const QString& userid, const QString &password)
{
    QString sql = QString("update localuserinfo set userpw='%1' where userid='%2'").arg(password, userid);

    QSqlQuery sql_query;
    sql_query.prepare(QString("select * from localuserinfo where userid='%1'").arg(userid));
    sql_query.exec();
    if(sql_query.next())
    {
        sql_query.prepare(sql);
        sql_query.exec();
    }
    else
    {
        sql_query.prepare(QString("insert into localuserinfo(userid, userpw) values('%1', '%2')").arg(userid, password));
        sql_query.exec();
    }
}

void DataBase::setFriendList(QList<QVector<QString>> friends)
{
    QString sql = "insert into friendlist(friendid, username, remark, "
                  "personalizedsignature, grouptype) values('%1', '%2', '%3', '%4', '%5')";

    QSqlQuery sql_query;

    for (QVector<QString> onefriend : friends)
    {
        QString s = sql.arg(onefriend[0], onefriend[1], onefriend[2], onefriend[3], onefriend[4]);
        qDebug() << s;
        sql_query.prepare(s);
        //sql_query.exec();
    }

    qDebug() << "?/";
}
QList<QVector<QString>> DataBase::getFriendList()
{
    QList<QVector<QString>> friends;

    QString sql = "select username, remark, grouptype, personalizedsignature from friendlist;";

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.exec();

    while(sql_query.next())
    {
        QString username= sql_query.value(0).toString();
        QString remarke = sql_query.value(1).toString();
        QString grouptype = sql_query.value(2).toString();
        QString personalizedsignature = sql_query.value(3).toString();

        QVector<QString> oneFriend;
        oneFriend.append(username);
        oneFriend.append(remarke);
        oneFriend.append(grouptype);
        oneFriend.append(personalizedsignature);

        friends.append(oneFriend);
    }

    return friends;
}

QStringList DataBase::getGroup()
{
    QStringList groups;
    QString sql = "select distinct grouptype from friendlist;";

    QSqlQuery sql_query;
    sql_query.prepare(sql);
    sql_query.exec();

    while(sql_query.next())
    {
        groups << sql_query.value(0).toString();
    }

    return groups;
}
