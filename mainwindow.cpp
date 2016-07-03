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
    connect(ui->treeWidget,&QTreeWidget::currentItemChanged,this,[=](QTreeWidgetItem* next,QTreeWidgetItem* prev){

        saveeditting(prev);

        //load selected state
        if(auto statedef = dynamic_cast<States*>(next)){
            int state = statedef->getstate();
            ui->substitutions->setelems(data.cns[state].first.substitutions);
        }
        else if(auto statebody = dynamic_cast<StateTreeItem*>(next)){
            auto parent = dynamic_cast<States*>(statebody->parent());
            int state = parent->getstate();
            int at = parent->findchild(statebody);
            ui->triggers->setelems(data.cns[state].second[at].triggers);
            ui->substitutions->setelems(data.cns[state].second[at].substitutions);
        }


    });

    connect(ui->treeWidget,&QTreeWidget::itemDoubleClicked,this,[=](QTreeWidgetItem* item,int){
        if(auto i = dynamic_cast<States*>(item)){
            auto pre = i->getstate();
            createstatedialog([=](int next){
               if( findstate(next).size() == 0){
                   i->setstate(next);
                   renameNumberOfState(pre,next);
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

    auto body = new StateTreeItem();
    body->setFlags(body->flags() | Qt::ItemIsEditable);
    body->setText(0,"comment");

    QVariant state;
    if(auto statedef = dynamic_cast<States*>(nowselect)){
        statedef->addChild(body);
        state = statedef->getstate();
    }else if(auto statebody = dynamic_cast<StateTreeItem*>(nowselect)){
        auto statedef = dynamic_cast<States*>(statebody->parent());
        statedef->addChild(body);
        state = statedef->getstate();
    }
    if(!state.isNull()){
    data.cns[state.toInt()].second.append(*(new CNS::StateBody()));
    }
}


void MainWindow::JsonExport(){
    QFile json(tr("test.json"));
    if(!json.open(QIODevice::WriteOnly)){
        return;
    }

    saveeditting(ui->treeWidget->currentItem());

    QJsonArray cns;

    auto keys = data.cns.keys();
    for(auto it = keys.begin(),end = keys.end(); it != end; it++){
        auto key = *it;

        QJsonArray defsubss;
        for(auto& subsdata : data.cns[key].first.substitutions){
            QJsonObject subs;
            subs["left"] = subsdata.first;
            subs["right"] = subsdata.second;
            defsubss.append(subs);
        }

        QJsonArray statebody;
        for(auto& bodyelem: data.cns[key].second){
            QJsonArray bodytrigs;
            for(auto& trig: bodyelem.triggers){
            bodytrigs.append(QJsonValue{trig});
            }

            QJsonArray bodysubss;
            for(auto& subsdata : bodyelem.substitutions){
                QJsonObject subs;
                subs["left"] = subsdata.first;
                subs["right"] = subsdata.second;
                bodysubss.append(subs);
            }
            QJsonObject body;
            body["triggers"] = QJsonValue(bodytrigs);
            body["substitutions"] = QJsonValue(bodysubss);
            statebody.append(body);
        }
        QJsonObject statedef;
        statedef["substitutions"] = QJsonValue(defsubss);
        statedef["comment"] = "defcomment";
        QJsonObject statedata;
        statedata["StateDef"] = QJsonValue(statedef);
        statedata["States"] = QJsonValue(statebody);
        QJsonObject cnselem;
        cnselem["StateNum"] = key;
        cnselem["StateData"] = statedata;
        cns.append(QJsonValue(cnselem));
    }

    QJsonDocument doc(cns);
    json.write(doc.toJson());
}

void MainWindow::saveeditting(QTreeWidgetItem* elem){
    //save now editting
    if(auto statedef = dynamic_cast<States*>(elem)){
        int state = statedef->getstate();
        data.cns[state].first.substitutions = ui->substitutions->getsubs();
    }
    else if(auto statebody = dynamic_cast<StateTreeItem*>(elem)){
        auto parent = dynamic_cast<States*>(statebody->parent());
        int state = parent->getstate();
        int at = parent->findchild(statebody);
        data.cns[state].second[at].triggers = ui->triggers->gettriggers();
        data.cns[state].second[at].substitutions = ui->substitutions->getsubs();
    }
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

 void MainWindow::renameNumberOfState(int pre,int next){
     data.cns[next] = data.cns[pre];
     data.cns.remove(pre);
 }
