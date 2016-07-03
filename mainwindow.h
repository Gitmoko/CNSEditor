#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QList>
#include "csubs.h"
#include "child.h"
#include "QTreeWidgetItem"
#include <functional>

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

public:
    Data data;

private:
    void savenoweditting(int state);
    void createstatedialog(std::function<void(int)> f);
    void changeDataState(int pre,int next);
    QList<QTreeWidgetItem*> findstate(int r);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
