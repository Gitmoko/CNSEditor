#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QList>
#include "csubs.h"
#include "child.h"
#include "QTreeWidgetItem"
#include <functional>
#include "cns.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    using Data = QMap<int,QPair<QList<QString>,QList<QPair<QString,QString>>>>;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void statesadd();
    void statebodyadd();
    void JsonExport();
    void JsonImport();

public:
    void addtype();
    void statebodyadd(int state,std::string tag = "name",CNS::StateBody *data = nullptr);
    void removeState();
    void swapStateBody(int state,int index1,int index2);
public:
    CNS::CNS data;

private:
    void saveeditting(QTreeWidgetItem*);
    void loadEditor(QTreeWidgetItem*);
    void createstatedialog(std::function<void(int)> f);
    void renameNumberOfState(int pre,int next);
    QList<QTreeWidgetItem*> findstate(int r);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
