#ifndef CTRIGGERS_H
#define CTRIGGERS_H

#include <QWidget>
#include <QList>

namespace Ui {
class CSubs;
}

class CSubs : public QWidget
{
    Q_OBJECT

public:
    explicit CSubs(QWidget *parent = 0);
    ~CSubs();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CSubs *ui;
public:
    QList<QPair<QString,QString>> getsubs()const;
    void setempty()const;
    void setelems(const QList<QPair<QString,QString>>&)const;
};

#endif // CTRIGGERS_H
