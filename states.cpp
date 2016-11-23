#include "states.h"

States::States(QTreeWidget *parent) :
    QTreeWidgetItem(parent)
{
    this->setFlags(this->flags() | Qt::ItemIsEditable);
}

States::States(int r,QString tag_,QTreeWidget *parent) :
    QTreeWidgetItem(parent)
{
    tag = tag_;
    number = r;
    setstate(r);
}
bool States::operator <(const QTreeWidgetItem& other)const {
    auto a = this->number;
    auto b = dynamic_cast<const States*>(&other)->getstate();
    return a < b;
}

void States::setstate(int r){
     number = r;
     update_text();
}

void States::settag(QString tag_){
     tag = tag_;
     update_text();
}

void States::update_text(){
    this->setText(0,QString::number(number) + " " + tag);
}

int States::getstate()const{
    return number;
}
QString States::gettag()const{
    return tag;
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
