#include "states.h"

States::States(QTreeWidget *parent) :
    QTreeWidgetItem(parent)
{
    this->setFlags(this->flags() | Qt::ItemIsEditable);
}

States::States(int r,QTreeWidget *parent) :
    QTreeWidgetItem(parent)
{
    this->setFlags(this->flags() | Qt::ItemIsEditable);
    this->setText(0,QString::number(r));
}
bool States::operator <(const QTreeWidgetItem& other)const {
    auto a = this->text(0).toInt();
    auto b = other.text(0).toInt();
    return a < b;
}

States::~States(){

}
