#include "addstatesdialog.h"
#include "ui_addstatesdialog.h"

addstatesdialog::addstatesdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addstatesdialog)
{
    ui->setupUi(this);
    connect(this,&QDialog::accepted,[=](){
        emit this->newstatenumber(ui->inputstate->value());
    });
}

addstatesdialog::~addstatesdialog()
{

    delete ui;
}
