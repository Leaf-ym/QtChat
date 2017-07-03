#include "basicwidget.h"
#include "skinmanagewidget.h"
#include "BasicControls/pushbutton.h"

#include <QMouseEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QApplication>
#include <QLabel>
#include <QDebug>
#include <QPixmap>
#include <QPushButton>
BasicWidget::BasicWidget(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setMouseTracking(true); // 响应鼠标移动

    color = QColor(40,138,221);
    backgroundColor = Qt::white;

    pressed=false;
    pressed2 = false;
    orientation = NONE;
    setMinimumSize(300,600);

    btn_close = new QPushButton(this);
    btn_close->setToolTip("关闭");
    btn_close->setObjectName("btn_close");
    btn_close->setFixedSize(28, 28);


    widgetIcon = new QLabel(this);
    widgetIcon->setFixedSize(28, 28);
    widgetIcon->setPixmap(QPixmap(":/Resource/js-linux.png").scaled(28, 28,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    widgetTitle = new QLabel(this);
    widgetTitle->setFixedHeight(28);
    widgetTitle->setObjectName("widgetTitle");
    connect(btn_close, &PushButton::clicked, this, &QWidget::hide);
}

BasicWidget::~BasicWidget()
{
    adjustment = true;
}

void BasicWidget::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);
    setOrientation(e);

    if (pressed) {
        QPoint p = e->globalPos();
        this->move(p.x() - point.x(), p.y() - point.y());
    }
    else if (pressed2 && orientation != NONE)
    {
        setSize(e);
    }
}

void BasicWidget::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);

    point = e->pos();
    rect = this->geometry();

    if (e->button() == Qt::LeftButton && orientation == NONE) {
        pressed = true;
    }
    else if(e->button() == Qt::LeftButton && orientation != NONE)
        pressed2 = true;
}

void BasicWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);

    if (e->button() == Qt::LeftButton) {
        pressed = pressed2 = false;
        rect = this->geometry();
    }
    pressed = false;
}

void BasicWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.setBrush(color);
    p.drawRect(0, 0, width(), 28);
    p.setBrush(backgroundColor);
    p.drawRect(0, 29, width(), height());
}

void BasicWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    btn_close->move(width() - btn_close->width(), 0);
    widgetIcon->move(0, 0);
    widgetTitle->move(33, 0);
    widgetTitle->resize(width()-100, 28);
}

void BasicWidget::enterEvent(QEvent *event)
{
}

void BasicWidget::leaveEvent(QEvent *event)
{
}

void BasicWidget::setAdjustmentSize(bool _adjustment)
{
    adjustment = _adjustment;
}

//设置方向
void BasicWidget::setOrientation(QMouseEvent *e)
{
    QPoint pp = e->pos();

    if (adjustment && !pressed2)
    {
        if ((pp.x() < 6 && pp.y() < 6))  // 左上
        {
            setCursor(Qt::SizeFDiagCursor);
            orientation = LEFTTOP;
        }
        else if((pp.x() > width() - 6 && pp.y() > height() - 6)) // 右下
        {
            setCursor(Qt::SizeFDiagCursor);
            orientation = RIGHTBOTTOM;
        }
        else if (pp.x() > width() - 6 && pp.y() < 6 ) // 右上
        {
            setCursor(Qt::SizeBDiagCursor);
            orientation = RIGHTTOP;
        }
        else if ( pp.x() < 6 && pp.y() > height() - 6) //左下
        {
            setCursor(Qt::SizeBDiagCursor);
            orientation = LEFTBOTTOM;
        }
        else if ( (pp.x() > 6 && pp.y() < 6) ) // 正上
        {
            setCursor(Qt::SizeVerCursor);
            orientation = TOP;
        }
        else if( ((pp.x() > 6) && (pp.y() > height() - 6)) ) // 正下
        {
            setCursor(Qt::SizeVerCursor);
            orientation = BOTTOM;
        }
        else if ( (pp.x() < 6 && pp.y() > 6)) //左
        {
            setCursor(Qt::SizeHorCursor);
            orientation = LEFT;
        }
        else if(((pp.x() > width() - 6) && (pp.y() > 6)) ) //右
        {
            setCursor(Qt::SizeHorCursor);
            orientation = RIGHT;

        }else
        {
            setCursor(Qt::ArrowCursor);
            orientation = NONE;
        }
    }

}

//根据拖动方向调整尺寸
void BasicWidget::setSize(QMouseEvent *e)
{
    if (!pressed2)
        return;

    QPoint gp = e->globalPos();

    QPoint ltp = rect.topLeft();
    QPoint rbp = rect.bottomRight();
    QPoint lbp = rect.bottomLeft();
    QPoint rtp = rect.topRight();

    switch(orientation)
    {
    case RIGHT:
        rbp.setX(gp.x() + rect.width() - point.x());
        break;
    case LEFT:
        ltp.setX(gp.x() - point.x());
        break;
    case TOP:
        ltp.setY(gp.y() - point.y());
        break;
    case BOTTOM:
        rbp.setY(gp.y() + rect.height() - point.y());
        break;
    case LEFTBOTTOM:
        ltp.setX(gp.x() - point.x());
        rbp.setY(gp.y() + rect.height() - point.y());
        break;
    case LEFTTOP:
        ltp.setX(gp.x() - point.x());
        ltp.setY(gp.y() - point.y());
        break;
    case RIGHTBOTTOM:
        rbp.setX(gp.x() + rect.width() - point.x());
        rbp.setY(gp.y() + rect.height() - point.y());
        break;
    case RIGHTTOP:
        rbp.setX(gp.x() + rect.width() - point.x());
        ltp.setY(gp.y() - point.y());
        break;
    default:
        break;
    }


    this->setGeometry(QRect(ltp, rbp));
}

void BasicWidget::setWidgetTitle(const QString &title)
{
    widgetTitle->setText(title);
}

void BasicWidget::setTitleBackgroundColor(QColor _color)
{
    color = _color;
    update();
}

void BasicWidget::setBackgroundColor(QColor color)
{
    backgroundColor = color;
    update();
}
