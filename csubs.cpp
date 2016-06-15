#include "csubs.h"
#include "ui_csubs.h"
#include <QLineEdit>
#include <QGridLayout>

CSubs::CSubs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CSubs)
{
    ui->setupUi(this);
}

CSubs::~CSubs()
{
    delete ui;
}

void CSubs::on_pushButton_clicked()
{
    auto W = new QWidget();
    auto vh = new QHBoxLayout();
    vh->setMargin(0);
    W->setLayout(vh);
    auto l1 = new QLineEdit();
    l1->setMinimumHeight(30);
    auto l2 = new QLineEdit();
    l2->setMinimumHeight(30);
    vh->addWidget(l1);
    vh->addWidget(l2);
    //this->ui->textsarea->widget()->layout()->addWidget(w);
    QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(this->ui->substitutions->widget()->layout());
    //verticalspacerの手前に要素を入れなければならない。vlayoutの添え字は0から始まる。

    auto count = layout->count()-1;
    layout->insertWidget(count,W);
}

QList<QPair<QString, QString>> CSubs::getsubs()const{
    QList<QPair<QString, QString>> ret;
    auto layout = this->ui->substitutions->widget()->layout();
    int max = layout->count()-2;
    for(int i = 0;i <= max;i++){
        QPair<QString,QString> elem;
        auto h = dynamic_cast<QHBoxLayout*>(layout->itemAt(i)->widget()->layout());
        elem.first = dynamic_cast<QLineEdit*>(h->itemAt(0)->widget())->text();
        elem.second = dynamic_cast<QLineEdit*>(h->itemAt(1)->widget())->text();
        ret.push_back(elem);
    }
    return ret;
}

void CSubs::setempty()const{
    auto layout = ui->substitutions->widget()->layout();
    while(layout->count() > 1){
        delete layout->itemAt(0)->widget();
   }
}


void CSubs::setelems(const QList<QPair<QString,QString>>& arg)const{
    setempty();
    auto vlayout = dynamic_cast<QVBoxLayout*>(ui->substitutions->widget()->layout());

    int test = vlayout->count();
    for(int i = 0;i<arg.size();i++){
        auto hlayout = new QHBoxLayout();
        hlayout->setMargin(0);
        auto w = new QLineEdit();
        w->setMinimumHeight(30);
        w->setText(arg.at(i).first);
        hlayout->insertWidget(0,w);

        auto w2 = new QLineEdit();
        w2->setMinimumHeight(30);
        w2->setText(arg.at(i).second);
        hlayout->insertWidget(1,w2);

        auto widget = new QWidget();
        widget->setLayout(hlayout);
        vlayout->insertWidget(vlayout->count()-1,widget);
    }

}

