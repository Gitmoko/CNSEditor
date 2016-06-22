#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "states.h"
#include "addstatesdialog.h"
#include "QJsonDocument"
#include "QJsonObject"
#include "QJsonArray"
#include "QFile"
#include "QDebug"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget->addAction(ui->actionAdd);
    connect(ui->treeWidget,&QTreeWidget::currentItemChanged,this,[=](QTreeWidgetItem* next,QTreeWidgetItem* prev){
        if(prev != nullptr){
            int prevstate = prev->text(0).toInt();
            this->savenoweditting(prevstate);


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

void MainWindow::JsonExport(){
    QFile json(tr("test.json"));
    if(!json.open(QIODevice::WriteOnly)){
        return;
    }

    savenoweditting( ui->treeWidget->currentItem()->text(0).toInt());

    QJsonArray cns;

    auto keys = data.keys();
    for(auto it = keys.begin(),end = keys.end(); it != end; it++){
        auto key = *it;
        QJsonArray trigs;
        for(auto& trig: data[key].first){
            trigs.append(QJsonValue{trig});
        }

        QJsonArray subss;
        for(auto& subsdata : data[key].second){
            QJsonObject subs;
            subs["left"] = subsdata.first;
            subs["right"] = subsdata.second;
            subss.append(subs);
        }
        QJsonObject state;
        state["state"] = key;
        state["triggers"] = QJsonValue{trigs};
        state["substitutions"] = QJsonValue{subss};
        cns.append(QJsonValue{state});
    }

    QJsonDocument doc(cns);
    json.write(doc.toJson());
}

void MainWindow::savenoweditting(int state){
    data[state].first = ui->widget->gettriggers();
    data[state].second = ui->widget_2->getsubs();
}
