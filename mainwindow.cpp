#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "states.h"
#include "addstatesdialog.h"
#include "QJsonDocument"
#include "QJsonObject"
#include "QJsonArray"
#include "QFile"
#include "QDebug"
#include "statetreeitem.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget->addAction(ui->actionAdd);
    connect(ui->treeWidget,&QTreeWidget::currentItemChanged,this,[=](QTreeWidgetItem* next,QTreeWidgetItem* prev){


        if(prev != nullptr){
            auto d = prev->columnCount();
            if(d == 2)
                return;

            int prevstate = prev->text(0).toInt();
            this->savenoweditting(prevstate);


            int nextstate = next->text(0).toInt();
            auto& trig = data[nextstate].first;
            auto& subs = data[nextstate].second;

            this->ui->widget->setelems(trig);
            this->ui->widget_2->setelems(subs);
        }


    });

    connect(ui->treeWidget,&QTreeWidget::itemDoubleClicked,this,[=](QTreeWidgetItem* item,int){
        if(auto i = dynamic_cast<States*>(item)){
            auto pre = i->getstate();
            createstatedialog([=](int next){
               if( findstate(next).size() == 0){
                   i->setstate(next);
                   changeDataState(pre,next);
               }
            });
        }

    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::statesadd(){
    createstatedialog([=](int r){
       if(findstate(r).size() == 0){
            auto s = new States(r);
            ui->treeWidget->addTopLevelItem(s);
            ui->treeWidget->sortItems(0,Qt::SortOrder::AscendingOrder);
        }
    });
}

void MainWindow::statebodyadd(){
    auto nowselect = ui->treeWidget->currentItem();
    if(nowselect != nullptr){
        auto body = new StateTreeItem();
        body->setFlags(body->flags());
        body->setText(0,"comment");
        nowselect->addChild(body);
    }
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

QList<QTreeWidgetItem*> MainWindow::findstate(int r){
    return ui->treeWidget->findItems(QString::number(r),Qt::MatchFlag::MatchExactly,0);
}
 void MainWindow::createstatedialog(std::function<void(int)> f){
     this->setEnabled(false);
     auto p = new addstatesdialog(this);
     p->setEnabled(true);
     p->show();
     connect(p,&addstatesdialog::newstatenumber,f);
     connect(p,&QDialog::finished,
             [=](int ){
         this->setEnabled(true);
     });

 }

 void MainWindow::changeDataState(int pre,int next){
     data[next] = data[pre];
     data.remove(pre);
 }
