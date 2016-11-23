#include "addstatesdialog.h"
#include "ui_addstatesdialog.h"

addstatesdialog::addstatesdialog(QWidget *parent,int statenum,QString tag) :
    QDialog(parent),
    ui(new Ui::addstatesdialog)
{
    ui->setupUi(this);
    ui->inputstate->setValue(statenum);
    ui->inputtag->setText(tag);
    connect(this,&QDialog::accepted,[=](){
        auto value = ui->inputstate->value();
        auto tag = ui->inputtag->text();
        emit this->newstatedata(value,tag);
        delete this;
    });
}

addstatesdialog::~addstatesdialog()
{

    delete ui;
}
