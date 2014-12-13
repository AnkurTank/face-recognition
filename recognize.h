#ifndef RECOGNIZE_H
#define RECOGNIZE_H

#include <QDialog>

namespace Ui {
class Recognize;
}

class Recognize : public QDialog
{
    Q_OBJECT

public:
    explicit Recognize(QWidget *parent = 0);
    ~Recognize();

protected:
    void closeEvent(QCloseEvent *);

private:
    Ui::Recognize *ui;
signals:
    void recognize_closed();
private slots:
    void on_btnBack_clicked();
};

#endif // RECOGNIZE_H
