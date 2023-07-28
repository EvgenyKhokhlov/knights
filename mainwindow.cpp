#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QVector<QString> knightsNames;
    knightsNames << "John" << "Rob" << "Artur" << "Clark" << "Luter";

    knightsCount = 5;
    Knife *knifeForLastKnight;
    for(int i = 0; i < knightsCount; i++){
        Knight* knight = new Knight(i, knightsNames.at(i));
        connect(knight, SIGNAL(stateChanged(QString)), this, SLOT(messageRecieve(QString)));
        knights.append(knight);

        Knife *knife = new Knife;
        knife->id = i;
        knight->setLeftKnife(knife);

        if(i != 0)
            knights.at(i - 1)->setRightKnife(knife);
        else
            knifeForLastKnight = knife;
        if(i == knightsCount - 1)
            knight->setRightKnife(knifeForLastKnight);
    }

    for(int i = 0; i < knights.count(); i++){
        knights.at(i)->start();
    }

    initTable();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTableData()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    for(int i = 0; i < knights.count(); i++){
        knights.at(i)->isSimulationAllowed = true;
    }
}

void MainWindow::on_stopButton_clicked()
{
    for(int i = 0; i < knights.count(); i++){
        knights.at(i)->isSimulationAllowed = false;
    }
}

void MainWindow::messageRecieve(QString message)
{
    ui->textBrowser->append(message);
}

void MainWindow::updateTableData()
{
    QTableWidget *table = ui->tableWidget;

    QString state;
    int condition;
    for (int i = 0; i < knightsCount; i++) {
        condition = knights.at(i)->getCondition();
        if(condition == 0) state = "Eating";
        if(condition == 1) state = "Story Telling";
        if(condition == 2) state = "Idle";

        table->setItem(i, 2, new QTableWidgetItem(state));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(knights.at(i)->getHungerLevel())));
    }
}

void MainWindow::initTable()
{
    QTableWidget *table = ui->tableWidget;

    table->setRowCount(knightsCount);

    QStringList horzHeaders;
    horzHeaders << "No" << "Name" << "Condition" << "Hunger" << "Left knife"
                << "Right knife" << "Food intake" << "Stories told";
    table->setColumnCount(horzHeaders.count());

    // выставляем размеры таблицы относительно контента
    const int sizeRatio = 2;
    table->setFixedHeight(table->rowHeight(0) * knightsCount + table->horizontalHeader()->height() + sizeRatio);
    int width = 0;
    for (int i = 0; i < table->columnCount(); i++) {
        width += table->columnWidth(i);
    }
    table->setFixedWidth(width + sizeRatio);

    table->setHorizontalHeaderLabels(horzHeaders);
    table->verticalHeader()->hide();
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for (int i = 0; i < knightsCount; i++) {
        table->setItem(i, 0, new QTableWidgetItem(QString::number(knights.at(i)->getId())));
        table->setItem(i, 1, new QTableWidgetItem(knights.at(i)->getName()));
    }
}
