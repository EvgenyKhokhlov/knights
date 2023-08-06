#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создаем хранилище состояний рыцарей
    knightsData = new KnightsData;
    connect(knightsData, SIGNAL(knightState(int, int)), this, SLOT(knightStateChanged(int, int)));

    // Создаем рыцарей и расставляем ножи
    Knife *knifeForLastKnight;
    for(int i = 0; i < knightsData->getKnightsCount(); i++){
        Knight* knight = new Knight(i, knightsData->getNames().at(i));
        connect(knight, SIGNAL(stateChanged(int, int, int)), knightsData, SLOT(knightStateChanged(int, int, int)));
        connect(this, SIGNAL(setSimulation(bool)), knight, SLOT(setIsSimulationAllowed(bool))); //TODO перенести в алгоритм
        knights.append(knight);

        Knife *knife = new Knife;
        knife->id = i;
        knight->setLeftKnife(knife);

        if(i != 0)
            knights.at(i - 1)->setRightKnife(knife);
        else
            knifeForLastKnight = knife;
        if(i == knightsData->getKnightsCount() - 1) // Выдаем последнему рыцарю правый нож
            knight->setRightKnife(knifeForLastKnight);
    }

    initTable();

    for(int i = 0; i < knights.count(); i++){
        knights.at(i)->start();
    }

    QTimer *tableUpdateTimer = new QTimer(this);
    connect(tableUpdateTimer, SIGNAL(timeout()), this, SLOT(updateTableData()));
    tableUpdateTimer->start(1000 * updateTimeSecs);

    // Создаем таймер для временной блокировки кнопок
    buttonEnableTimer = new QTimer;
    connect(buttonEnableTimer, SIGNAL(timeout()), this, SLOT(buttonEnable()));
    buttonEnableTimer->setSingleShot(true);

    // Создаем графическую сцену
    scene = new Scene(0, 0, ui->graphicsView->geometry().width() - 2,
                      ui->graphicsView->geometry().height() - 2, knightsData->getNames());
    connect(knightsData, SIGNAL(knightState(int, int)), scene, SLOT(knightStateChanged(int, int)));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    ui->stopButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    emit setSimulation(true);
    scene->startSimulation();

    isStartButtonEnableRequired = false;
    ui->startButton->setEnabled(false);
    buttonEnableTimer->start(1000 * buttonDisabledDelaySecs);
}

void MainWindow::on_stopButton_clicked()
{
    emit setSimulation(false);
    scene->stopSimulation();

    isStartButtonEnableRequired = true;
    ui->stopButton->setEnabled(false);
    buttonEnableTimer->start(1000 * buttonDisabledDelaySecs);
}

void MainWindow::updateTableData()
{
    for(int i = 0; i < knightsData->getKnightsCount(); i++){
        QString state;
        int condition = knightsData->getCondition().at(i);
        if(condition == 0) state = "Eating";
        if(condition == 1) state = "StoryTelling";
        if(condition == 2) state = "LeftKnifeTaking";
        if(condition == 3) state = "RightKnifeTaking";
        if(condition == 4) state = "Idle";

        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(state));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(knightsData->getHunger().at(i))));

        updateKnifeCondition(knightsData->getLeftKnifeInHand().at(i), i, 4);
        updateKnifeCondition(knightsData->getRightKnifeInHand().at(i), i, 5);

        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(knightsData->getMealsNumber().at(i))));
        ui->tableWidget->setItem(i, 7, new QTableWidgetItem(QString::number(knightsData->getStoriesToldNumber().at(i))));
    }
}

void MainWindow::knightStateChanged(int id, int state)
{
    // Формируем сообщение о состоянии рыцаря
    QString message;
    message.append(knightsData->getNames().at(id));
    if(state == 0) message.append(" eating");
    if(state == 1) message.append(" tells a story");
    if(state == 2) message.append(" taking the left knife");
    if(state == 3) message.append(" taking the right knife");

    ui->textBrowser->append(message);
}

void MainWindow::buttonEnable()
{
    if(isStartButtonEnableRequired)
        ui->startButton->setEnabled(true);
    else
        ui->stopButton->setEnabled(true);
}

void MainWindow::initTable()
{
    QTableWidget *table = ui->tableWidget;
    
    table->setRowCount(knightsData->getKnightsCount());

    QStringList horzHeaders;
    horzHeaders << "No" << "Name" << "Condition" << "Hunger" << "Left knife"
                << "Right knife" << "Food intake" << "Stories told";
    table->setColumnCount(horzHeaders.count());

    // выставляем размеры таблицы относительно контента
    const int sizeRatio = 2;
    table->setFixedHeight(table->rowHeight(0) * knightsData->getKnightsCount() +
                          table->horizontalHeader()->height() + sizeRatio);
    int width = 0;
    for (int i = 0; i < table->columnCount(); i++) {
        width += table->columnWidth(i);
    }
    table->setFixedWidth(width + sizeRatio);

    table->setHorizontalHeaderLabels(horzHeaders);
    table->verticalHeader()->hide();

    // Для перехвата клика мышки - создадим пустой виджет поверх таблицы
    QWidget *tableClickCatcher = new QWidget(this);
    tableClickCatcher->setFixedSize(table->geometry().width(), table->geometry().height());
    tableClickCatcher->move(table->geometry().x(), table->geometry().y());

    for (int i = 0; i < knightsData->getKnightsCount(); i++) {
        table->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        table->setItem(i, 1, new QTableWidgetItem(knightsData->getNames().at(i)));
    }
}

void MainWindow::updateKnifeCondition(bool isInHand, int id, int tableColumn)
{
    QString condition;
    if(isInHand)
        condition = "In Hand";
    else
        condition = "On the table";

    ui->tableWidget->setItem(id, tableColumn, new QTableWidgetItem(condition));
}
