#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "game.h"
#include <QMainWindow>
#include "help.h"
#include <QGridLayout>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class GameField;
class Help;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GameField *gameField;
    Help *helpField;
    QGridLayout *gridLayout;
    QWidget *centralWidget;

};
#endif // MAINWINDOW_H
