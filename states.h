#ifndef STATES_H
#define STATES_H

#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui {
class States;
}

class States : public QTreeWidgetItem
{

    int number;
    QString tag;
    void update_text();
public:
    explicit States(QTreeWidget *parent = 0);
    explicit States(int r,QString tag = "",QTreeWidget *parent = 0);
    virtual bool operator < (const QTreeWidgetItem & other) const;
    void setstate(int);
    void settag(QString);
    int getstate()const;
    QString gettag()const;
    int findchild(QTreeWidgetItem*);
    ~States();

};

#endif // STATES_H
