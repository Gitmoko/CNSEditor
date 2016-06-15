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
    void newstatenumber(int);

public:
    explicit addstatesdialog(QWidget *parent = 0);
    ~addstatesdialog();

private:
    Ui::addstatesdialog *ui;
};

#endif // ADDSTATESDIALOG_H
