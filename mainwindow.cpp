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
#include "MyParserSyntax.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto s = new States(0);
    ui->treeWidget->addTopLevelItem(s);
    ui->treeWidget->setCurrentItem(s);
    ui->triggers->setEnabled(false);

    connect(ui->removestate,&QPushButton::pressed,this,&MainWindow::removeState);

    connect(ui->treeWidget,&QTreeWidget::currentItemChanged,this,[=](QTreeWidgetItem* next,QTreeWidgetItem* prev){
        saveeditting(prev);
        loadEditor(next);
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

    auto upperaction = new QAction(tr("up"),ui->treeWidget);
    ui->treeWidget->addAction(upperaction);
    connect(upperaction,&QAction::triggered,this,[=](bool){
       auto body = dynamic_cast<StateTreeItem*>(ui->treeWidget->currentItem());
       if(!body)
            return;

       auto parent = dynamic_cast<States*>(body->parent());
       auto state = parent->getstate();
       int index = parent->findchild(body);
       saveeditting(body);
       swapStateBody(state,index,index-1);
       loadEditor(body);

    });

    auto downaction = new QAction(tr("down"),ui->treeWidget);
    ui->treeWidget->addAction(downaction);
    connect(downaction,&QAction::triggered,this,[=](bool){
       auto body = dynamic_cast<StateTreeItem*>(ui->treeWidget->currentItem());
       if(!body)
            return;

       auto parent = dynamic_cast<States*>(body->parent());
       auto state = parent->getstate();
       int index = parent->findchild(body);
       saveeditting(body);
       swapStateBody(state,index,index+1);
       loadEditor(body);

    });




    auto typeaction = new QAction(tr("add"),ui->typelist);
    ui->typelist->addAction(typeaction);
    connect(typeaction,&QAction::triggered,this,[=](bool){
        this->addtype();
    });

    connect(ui->addtype,&QPushButton::pressed,this,&MainWindow::addtype);

    auto removeaction = new QAction(tr("remove"),ui->typelist);
    ui->typelist->addAction(removeaction);
    connect(removeaction,&QAction::triggered,this,[=](bool){
        auto item = ui->typelist->currentItem();
        delete item;
    });

    connect(ui->removetype,&QPushButton::pressed,this,[=](){
        auto item = ui->typelist->currentItem();
        delete item;
    });

    auto importaction = new QAction(tr("import"),ui->menuMenu);
    ui->menuMenu->addAction(importaction);
    connect(importaction,&QAction::triggered,this,[=](bool){
        this->JsonImport();
    });


    try{
    auto e = MyParser::parse_impl_debug("{mp,\"ab\",42}");
    }catch(MyParser::compile_failed c){
        auto p = new QDialog(this);
        auto pos = c.pos;
        new QLabel(tr(pos.c_str()),p);
        p->show();
    }
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
            this->data.cns.insert(r,decltype(data.cns.value(r)){});
        }
    });
}

void MainWindow::statebodyadd(){
    auto nowselect = ui->treeWidget->currentItem();

    auto body = new StateTreeItem();
    body->setFlags(body->flags() | Qt::ItemIsEditable);
    body->setText(0,"name");

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
    data.cns[state.toInt()].second.append(CNS::StateBody());
    }
}

void MainWindow::statebodyadd(int state,std::string tag,CNS::StateBody* bodydata){
    auto body = new StateTreeItem();
    body->setFlags(body->flags() | Qt::ItemIsEditable);
    body->setText(0,QString::fromStdString(tag));

    this->findstate(state)[0]->addChild(body);
    data.cns[state].second.append(bodydata == nullptr ? CNS::StateBody() : *bodydata);
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
        QJsonArray deftypes;
        for(auto& typesdata : data.cns[key].first.Type){
            deftypes.append(typesdata);
        }
        QJsonObject statedef;
        statedef["substitutions"] = QJsonValue(defsubss);
        statedef["comment"] = data.cns[key].first.Comment;
        statedef["type"] = QJsonValue(deftypes);

        QJsonArray statebody;
        for(auto i = 0,end = data.cns[key].second.size();i<end;i++){
            auto& bodyelem = data.cns[key].second[i];
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
            QJsonArray bodytypes;
            for(auto& typesdata : bodyelem.Type){
                bodytypes.append(typesdata);
            }
            QJsonObject body;
            body["triggers"] = QJsonValue(bodytrigs);
            body["substitutions"] = QJsonValue(bodysubss);
            body["tag"] = QJsonValue(findstate(key)[0]->child(i)->text(0));
            body["type"] = QJsonValue(bodytypes);
            body["comment"] = data.cns[key].second[i].Comment;


            statebody.append(body);
        }


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



void MainWindow::JsonImport(){

    QFile jsonfile;
    jsonfile.setFileName("test.json");
    jsonfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(!jsonfile.isOpen()){
        return;
    }

    QJsonDocument json;
    QString jsondata = jsonfile.readAll();
    auto jsonutf = jsondata.toUtf8();
    QJsonParseError err;
    json = QJsonDocument::fromJson(jsonutf,&err);

    if(err.error != QJsonParseError::NoError){
        return;
    }


    ui->treeWidget->setCurrentItem(nullptr);
    ui->treeWidget->clear();
    data.cns.clear();
    QJsonArray cns = json.array();
    for(auto& elem: cns){
        QJsonObject cnselem = elem.toObject();
        auto key = cnselem["StateNum"].toInt();
        auto statedata = cnselem["StateData"].toObject();
        data.cns.insert(key,{});


        auto s = new States(key);
        ui->treeWidget->addTopLevelItem(s);

        auto statedef = statedata["StateDef"].toObject();
        auto statebody = statedata["States"].toArray();

        //Set statedef
        {
            auto subs = statedef["substitutions"].toArray();
            for(auto& elem:subs){
                auto elemobj = elem.toObject();
                data.cns[key].first.substitutions.push_back(QPair<QString,QString>{elemobj["left"].toString(),elemobj["right"].toString()});
            }
        }
        data.cns[key].first.Comment = statedef["comment"].toString();
        auto types = statedef["type"].toArray();
        for(auto& elem : types){
            data.cns[key].first.Type.push_back(elem.toString());
        }

        //Set statebody
        for(auto& body : statebody){
            CNS::StateBody bodydata;
            auto bodyobj = body.toObject();
            bodydata.Comment = bodyobj["comment"].toString();
            auto types = bodyobj["type"].toArray();
            for(auto& elem:types){
                bodydata.Type.push_back(elem.toString());
            }
            for(auto trig:bodyobj["triggers"].toArray()){
                bodydata.triggers.push_back(trig.toString());
            }
            for(auto subs:bodyobj["substitutions"].toArray()){
                auto subsobj = subs.toObject();
                bodydata.substitutions.push_back(QPair<QString,QString>{subsobj["left"].toString(),subsobj["right"].toString()});
            }
            statebodyadd(key,bodyobj["tag"].toString().toStdString(),&bodydata);
        }
    }



}

void MainWindow::saveeditting(QTreeWidgetItem* elem){
    if(auto statedef = dynamic_cast<States*>(elem)){
        int state = statedef->getstate();
        data.cns[state].first.substitutions = ui->substitutions->getsubs();
        data.cns[state].first.Comment = ui->comment->toPlainText();
        data.cns[state].first.Type.clear();
        for(auto i = 0,end = ui->typelist->topLevelItemCount();i<end;i++){
           data.cns[state].first.Type.push_back(ui->typelist->topLevelItem(i)->text(0));
        }
    }
    else if(auto statebody = dynamic_cast<StateTreeItem*>(elem)){
        auto parent = dynamic_cast<States*>(statebody->parent());
        int state = parent->getstate();
        int at = parent->findchild(statebody);
        data.cns[state].second[at].triggers = ui->triggers->gettriggers();
        data.cns[state].second[at].substitutions = ui->substitutions->getsubs();
        data.cns[state].second[at].Comment = ui->comment->toPlainText();
        data.cns[state].second[at].Type.clear();
        for(auto i = 0,end = ui->typelist->topLevelItemCount();i<end;i++){
           data.cns[state].second[at].Type.push_back(ui->typelist->topLevelItem(i)->text(0));
        }
    }
}

QList<QTreeWidgetItem*> MainWindow::findstate(int r){
    auto ret = ui->treeWidget->findItems(QString::number(r),Qt::MatchFlag::MatchExactly,0);
    return ret;
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
 void MainWindow::loadEditor(QTreeWidgetItem *next){

     if(auto statedef = dynamic_cast<States*>(next)){
         int state = statedef->getstate();
         ui->substitutions->setelems(data.cns[state].first.substitutions);
         ui->triggers->setempty();
         ui->triggers->setEnabled(false);
         ui->comment->setText(data.cns[state].first.Comment);
         for(auto i = 0,end = ui->typelist->topLevelItemCount();i < end;i++){
            ui->typelist->takeTopLevelItem(0);
         }
         for(auto i = 0,end = data.cns[state].first.Type.size();i < end;i++){
             auto item = new QTreeWidgetItem();
             item->setText(0,data.cns[state].first.Type[i]);
             item->setFlags(item->flags() | Qt::ItemIsEditable);
             ui->typelist->addTopLevelItem(item);
         }
     }
     else if(auto statebody = dynamic_cast<StateTreeItem*>(next)){
         auto parent = dynamic_cast<States*>(statebody->parent());
         int state = parent->getstate();
         int at = parent->findchild(statebody);
         ui->triggers->setelems(data.cns[state].second[at].triggers);
         ui->substitutions->setelems(data.cns[state].second[at].substitutions);
         ui->comment->setText(data.cns[state].second[at].Comment);
         ui->triggers->setEnabled(true);
         for(auto i = 0,end = ui->typelist->topLevelItemCount();i < end;i++){
            ui->typelist->takeTopLevelItem(0);
         }
         for(auto i = 0,end = data.cns[state].second[at].Type.size();i < end;i++){
             auto item = new QTreeWidgetItem();
             item->setText(0,data.cns[state].second[at].Type[i]);
             item->setFlags(item->flags() | Qt::ItemIsEditable);
             ui->typelist->addTopLevelItem(item);
         }

     }


 }

 void MainWindow::addtype(){
     auto item = new QTreeWidgetItem();
     item->setText(0,tr("type"));
     item->setFlags(item->flags() | Qt::ItemIsEditable);
     ui->typelist->addTopLevelItem(item);
 }

 void MainWindow::removeState(){
     auto target = ui->treeWidget->currentItem();
     if(auto statedef = dynamic_cast<States*>(target)){
         if(ui->treeWidget->topLevelItemCount() == 1){
             return;
         }
         int state = statedef->getstate();
         data.cns.remove(state);
         delete statedef;
     }
     else if(auto statebody = dynamic_cast<StateTreeItem*>(target)){
         auto parent = dynamic_cast<States*>(statebody->parent());
         int state = parent->getstate();
         int at = parent->findchild(statebody);
         if(parent->childCount() <= 1){
            ui->treeWidget->setCurrentItem(parent);
         }
         else{
             if(at == 0){
               ui->treeWidget->setCurrentItem(parent->child(1));
             }else{
                 ui->treeWidget->setCurrentItem(parent->child(at-1));
             }
         }
         data.cns[state].second.removeAt(at);
         delete statebody;
     }
 }

 void MainWindow::swapStateBody(int state,int index1,int index2){
     auto w1 = findstate(state)[0]->child(index1);
     auto w2 = findstate(state)[0]->child(index2);

     if((w1 == nullptr || w2 == nullptr) || (w1 == w2))
         return;

     auto tmp = w1->clone();
     *w1 = *w2;
     *w2 = *tmp;

     auto tmpdata = data.cns[state].second[index1];
     data.cns[state].second[index1] = data.cns[state].second[index2];
     data.cns[state].second[index2] = tmpdata;


 }
