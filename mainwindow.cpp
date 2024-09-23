#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gameField = new GameField();
    helpField = new Help();
    connect(gameField, &GameField::ChangeTextInHelpField, helpField, &Help::ChangeTextInField);
    gridLayout = new QGridLayout();
    centralWidget = new QWidget();
    gridLayout -> addWidget(gameField, 0, 0);
    gridLayout -> addWidget(helpField, 1, 0);
    centralWidget -> setLayout(gridLayout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
