#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QHeaderView>
#include <QTimer>

#include "knightsdata.h"
#include "hungercontrol.h"
#include "GraphicScene/scene.h"

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
    void updateTableData();
    void knightStateChanged(int id, int state);
    void buttonEnable();

signals:
    void setSimulation(bool input);

private:
    Ui::MainWindow *ui;
    KnightsData *knightsData;
    HungerControl *hungerControl;
    Scene *scene;
    QVector<Knight *> knights;
    QTimer *buttonEnableTimer;
    bool isStartButtonEnableRequired;

    void initTable();
    void updateKnifeCondition(bool isInHand, int id, int tableColumn);

    static const double updateTimeSecs = 0.5;
    static const double buttonDisabledDelaySecs = 3;
};

#endif // MAINWINDOW_H
