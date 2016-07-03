#ifndef STATES_H
#define STATES_H

#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui {
class States;
}

class States : public QTreeWidgetItem
{

public:
    explicit States(QTreeWidget *parent = 0);
    explicit States(int r,QTreeWidget *parent = 0);
    virtual bool operator < (const QTreeWidgetItem & other) const;
    void setstate(int);
    int getstate();
    ~States();

};

#endif // STATES_H
