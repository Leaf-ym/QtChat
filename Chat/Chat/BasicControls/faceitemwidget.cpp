#include "faceitemwidget.h"
#include <QMovie>
#include <QDebug>

FaceItemWidget::FaceItemWidget(QString fileName , QSize emotionMoiveSize)
	: QLabel(NULL)
{
	//���ȹ��캯���н��г�ʼ��;
	QMovie* iconMovie = new QMovie;
	iconMovie->setFileName(fileName);
	this->setMovie(iconMovie);
	setContentsMargins(3, 3, 3, 3);
	iconMovie->setScaledSize(QSize(emotionMoiveSize.width(), emotionMoiveSize.height()));
	//Ϊ�˶�̬ͼƬ������ʾ����������startͼƬ����ʾ��������stopͼƬһ�ִ��ڶ�̬Ч����
	iconMovie->start();
	iconMovie->stop();
	setStyleSheet("QLabel:hover{border: 1px solid rgb(111, 156, 207);\
								color: rgb(2, 65, 132);\
                                background: rgba(255, 255, 255, 200);}");
}

FaceItemWidget::~FaceItemWidget()
{}

// ������Label�¼�
void FaceItemWidget::enterEvent(QEvent* event)
{
	QMovie* movie = this->movie();
	movie->start();
	setContentsMargins(4, 2, 2, 4);
    QLabel::enterEvent(event);
}
// ����뿪Label�¼�
void FaceItemWidget::leaveEvent(QEvent* event)
{
	QMovie* movie = this->movie();
	movie->jumpToFrame(0);
	movie->stop();
	setContentsMargins(3, 3, 3, 3);
    QLabel::leaveEvent(event);
}
