#ifndef ADDSTATESDIALOG_H
#define ADDSTATESDIALOG_H

#include <QDialog>

namespace Ui {
class addstatesdialog;
}

class addstatesdialog : public QDialog
{
    Q_OBJECT

signals:
    void newstatedata(int,QString);

public:
    explicit addstatesdialog(QWidget *parent = 0,int statenum = 0,QString tag = "");
    ~addstatesdialog();

private:
    Ui::addstatesdialog *ui;
};

#endif // ADDSTATESDIALOG_H
