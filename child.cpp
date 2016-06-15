#include "child.h"
#include "ui_child.h"
#include "QLineedit.h"
#include "QVBoxLayout"

child::child(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::child)
{
    ui->setupUi(this);
}

child::~child()
{
    delete ui;
}


void child::on_addbutton_clicked()
{
    auto w = new QLineEdit();
    w->setMinimumHeight(30);
    //this->ui->textsarea->widget()->layout()->addWidget(w);
    QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(this->ui->textsarea->widget()->layout());
    //verticalspacerの手前に要素を入れなければならない。vlayoutの添え字は0から始まる。
    layout->insertWidget(layout->count()-1,w);

}
void child::on_removebutton_clicked()
{
    QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(this->ui->textsarea->widget()->layout());
    if(layout->count()>=2){
        //verticalspacerの手前に要素を入れなければならない。vlayoutの添え字は0から始まる。
        QWidget* w = layout->itemAt(layout->count()-2)->widget();
        delete w;
    }
}

QList<QString> child::gettriggers()const{
    QList<QString> ret;
    auto layout = ui->textsarea->widget()->layout();
    int max = layout->count()-2;
    for(int i = 0; i<=max;i++){
        ret.push_back(dynamic_cast<const QLineEdit*>(layout->itemAt(i)->widget())->text());
    }
    return ret;
}
void child::setempty()const{
    auto layout = ui->textsarea->widget()->layout();
    while(layout->count() > 1){
        delete layout->itemAt(0)->widget();
   }
}


void child::setelems(const QList<QString>& arg)const{
    setempty();
    for(int i = 0;i<arg.size();i++){
        auto layout = dynamic_cast<QVBoxLayout*>(ui->textsarea->widget()->layout());
        auto w = new QLineEdit();
        w->setMinimumHeight(30);
        w->setText(arg.at(i));
        layout->insertWidget(layout->count()-1,w);
    }

}
