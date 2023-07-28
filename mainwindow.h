#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "knight.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void messageRecieve(QString message);
    void updateTableData();

private:
    Ui::MainWindow *ui;
    QVector<Knight *> knights;
    int knightsCount;

    void initTable();
};

#endif // MAINWINDOW_H
