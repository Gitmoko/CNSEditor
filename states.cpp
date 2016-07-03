#include "states.h"

States::States(QTreeWidget *parent) :
    QTreeWidgetItem(parent)
{
    this->setFlags(this->flags() | Qt::ItemIsEditable);
}

States::States(int r,QTreeWidget *parent) :
    QTreeWidgetItem(parent)
{
    setstate(r);
}
bool States::operator <(const QTreeWidgetItem& other)const {
    auto a = this->text(0).toInt();
    auto b = other.text(0).toInt();
    return a < b;
}

void States::setstate(int r){
     this->setText(0,QString::number(r));
}

int States::getstate(){
    return this->text(0).toInt();
}

int States::findchild(QTreeWidgetItem* arg){
    int childnum = this->childCount();
    for(auto i = 0; i<childnum;i++){
     if(arg == this->child(i))
         return i;
    }
    return -1;
}

States::~States(){

}
