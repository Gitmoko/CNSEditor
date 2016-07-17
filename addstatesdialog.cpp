#include "addstatesdialog.h"
#include "ui_addstatesdialog.h"

addstatesdialog::addstatesdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addstatesdialog)
{
    ui->setupUi(this);
    connect(this,&QDialog::accepted,[=](){
        auto value = ui->inputstate->value();
        emit this->newstatenumber(value);
        delete this;
    });
}

addstatesdialog::~addstatesdialog()
{

    delete ui;
}
