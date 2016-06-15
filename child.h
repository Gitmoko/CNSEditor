#ifndef CHILD_H
#define CHILD_H

#include <QWidget>
#include <QSignalMapper>
#include <QList>

namespace Ui {
class child;
}

class child : public QWidget
{
    Q_OBJECT

public:
    explicit child(QWidget *parent = 0);
    ~child();
signals:
    void triggeradd();

public slots:
    void on_addbutton_clicked();
    void on_removebutton_clicked();

private:
    Ui::child *ui;

public:
    QList<QString> gettriggers()const;
    void setempty()const;
    void setelems(const QList<QString>&)const;
};

#endif // CHILD_H
