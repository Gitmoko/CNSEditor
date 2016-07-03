#ifndef STATETREEITEM_H
#define STATETREEITEM_H

#include <QWidget>
#include <QTreeWidgetItem>

class StateTreeItem : public QTreeWidgetItem
{

public:
    explicit StateTreeItem();
    ~StateTreeItem();

public:
    enum DATATYPE : int{STATEDEF,STATEBODY};
    StateTreeItem::DATATYPE tag = STATEDEF;

private:
};

#endif // STATETREEITEM_H
