#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "states.h"
#include "addstatesdialog.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget->addAction(ui->actionAdd);
    connect(ui->treeWidget,&QTreeWidget::currentItemChanged,this,[=](QTreeWidgetItem* next,QTreeWidgetItem* prev){
        if(prev != nullptr){
            int prevstate = prev->text(0).toInt();
            data[prevstate].first = ui->widget->gettriggers();
            data[prevstate].second = ui->widget_2->getsubs();


            int nextstate = next->text(0).toInt();
            auto& trig = data[nextstate].first;
            auto& subs = data[nextstate].second;

            this->ui->widget->setelems(trig);
            this->ui->widget_2->setelems(subs);
        }


    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::statesadd(){
    this->setEnabled(false);
    auto p = new addstatesdialog(this);
    p->setEnabled(true);
    p->show();
    connect(p,&addstatesdialog::newstatenumber,
            [=](int r){
       if( ui->treeWidget->findItems(QString::number(r),Qt::MatchFlag::MatchExactly).size() == 0){
        ui->treeWidget->addTopLevelItem(new States(r));
        ui->treeWidget->sortItems(0,Qt::SortOrder::AscendingOrder);
        }
    });
    connect(p,&QDialog::finished,
            [=](int r){
        this->setEnabled(true);
    });
}
