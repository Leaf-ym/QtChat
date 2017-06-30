#ifndef SEARCHLINEEDIT_H
#define SEARCHLINEEDIT_H



#include <QLineEdit>
class QLabel;
class QPushButton;

class SearchLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    SearchLineEdit(QWidget *parent = Q_NULLPTR);
    void hideButton();
protected slots:
    void textChange(const QString &text);

protected:
    void resizeEvent(QResizeEvent *);
    void focusOutEvent(QFocusEvent *e);
    void focusInEvent(QFocusEvent *e);

signals:
    void LineEditFocusIn();
    void LineEditFocusOut();
    void searachResult(const QVector<QStringList>&);
private:
    void init();

    QLabel *pix_label; // 显示图片
    QPushButton *btn_clear;
};
#endif // SEARCHLINEEDIT_H
