#include "game.h"
#include <QPainter>
#include <QList>
#include <QTimer>
#include <QKeyEvent>
#include <QRandomGenerator>

GameField::GameField()
    : gameSnake(nullptr), food(nullptr), snakeItemSize(10),
    moveSnakeTimer(nullptr), fieldSize(width() / snakeItemSize),
    isGameOnPause(false), gameScore(0), gameStatus(false)
{
    setFixedSize(400, 400);
    setFocusPolicy(Qt::StrongFocus);
    StartNewGame();
}


void GameField::paintEvent(QPaintEvent *e)
{
    QBrush snakeColor(QColor(102, 255, 0));
    QBrush foodColor(QColor(255, 0, 0));

    QPainter painter;
    painter.begin(this);

    if (gameStatus) {
        painter.setFont(QFont("Arial", 30, 700));
        painter.setPen(Qt::white);
        painter.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, "ВЫ ПРОИГРАЛИ!\nВаш счёт: " + QString::number(gameScore));
    } else {

        painter.drawRect(0, 0, width() - 1, height() - 1);
        painter.setBrush(snakeColor);

        for (int i = 0; i < gameSnake->snakeBody.size(); i++) {
            painter.drawEllipse(gameSnake->snakeBody[i]->getX() * snakeItemSize,
                                gameSnake->snakeBody[i]->getY() * snakeItemSize,
                                snakeItemSize, snakeItemSize);
        }

        painter.setBrush(foodColor);
        painter.drawEllipse(food->getX() * snakeItemSize,
                            food->getY() * snakeItemSize,
                            snakeItemSize, snakeItemSize);
    }

    painter.end();
}


void GameField::keyPressEvent(QKeyEvent *e)
{
    Snake::SnakeMoveDirection currentDirection = gameSnake->getMoveDirection();

    if ((e->key() == Qt::Key_Right || e->key() == Qt::Key_D) && currentDirection != Snake::left) {
        gameSnake->setMoveDirection(Snake::right);
    } else if ((e->key() == Qt::Key_Left || e->key() == Qt::Key_A) && currentDirection != Snake::right) {
        gameSnake->setMoveDirection(Snake::left);
    } else if ((e->key() == Qt::Key_Down || e->key() == Qt::Key_S) && currentDirection != Snake::up) {
        gameSnake->setMoveDirection(Snake::down);
    } else if ((e->key() == Qt::Key_Up || e->key() == Qt::Key_W) && currentDirection != Snake::down) {
        gameSnake->setMoveDirection(Snake::up);
    } else if (e->key() == Qt::Key_Space){
        if (gameStatus){
            StartNewGame();
            return;
        } else {
            isGameOnPause = !isGameOnPause;
            setGameOnPause(isGameOnPause);
        }

    }
}


void GameField::setGameOnPause(bool isGameOnPause)
{
    QString HelpText;

    if (isGameOnPause){
        moveSnakeTimer -> stop();
        HelpText = "Счёт: " + QString::number(gameScore) + "\nПродолжить - ПРОБЕЛ";
    } else {
        moveSnakeTimer -> start(100);
        HelpText = "Счёт: " + QString::number(gameScore) + "\nПауза - ПРОБЕЛ";
    }

    emit ChangeTextInHelpField(HelpText);

}

void GameField::GameOver()
{
    gameStatus = true;
    repaint();
    QString HelpText = "Начать заново - ПРОБЕЛ";
    emit ChangeTextInHelpField(HelpText);
    moveSnakeTimer -> stop();
    delete food;
    delete moveSnakeTimer;
    delete gameSnake;

}

void GameField::StartNewGame()
{
    gameSnake = new Snake();
    moveSnakeTimer = new QTimer();
    food = new SnakeItems(fieldSize / 2, fieldSize / 2);
    gameScore = 0;
    QString HelpText = "Счёт: " + QString::number(gameScore) + "\nПродолжить - ПРОБЕЛ";
    emit ChangeTextInHelpField(HelpText);
    gameStatus = false;
    isGameOnPause = false;
    connect(moveSnakeTimer, &QTimer::timeout, this, &GameField::MoveSnakeSlot);
    moveSnakeTimer -> start(100);

}

void GameField::CreateFood()
{

    delete food;
    food = nullptr;

    bool foodPositionValid = false;

    while (!foodPositionValid) {
        int randomX = QRandomGenerator::global()->bounded(0, fieldSize);
        int randomY = QRandomGenerator::global()->bounded(0, fieldSize);


        food = new SnakeItems(randomX, randomY);
        foodPositionValid = true;


        for (SnakeItems* snakePart : gameSnake->snakeBody) {
            if (food->getX() == snakePart->getX() && food->getY() == snakePart->getY()) {
                foodPositionValid = false;
                break;
            }
        }
    }
}


void GameField::MoveSnakeSlot()
{
    QList<SnakeItems*>& snakeBody = gameSnake->getSnakeBody();
    SnakeItems* newSnakeItem = nullptr;
    Snake::SnakeMoveDirection currentDirection = gameSnake->getMoveDirection();

    if (currentDirection == Snake::right) {
        newSnakeItem = new SnakeItems(snakeBody[0]->getX() + 1, snakeBody[0]->getY());
    } else if (currentDirection == Snake::left) {
        newSnakeItem = new SnakeItems(snakeBody[0]->getX() - 1, snakeBody[0]->getY());
    } else if (currentDirection == Snake::up) {
        newSnakeItem = new SnakeItems(snakeBody[0]->getX(), snakeBody[0]->getY() - 1);
    } else if (currentDirection == Snake::down) {
        newSnakeItem = new SnakeItems(snakeBody[0]->getX(), snakeBody[0]->getY() + 1);
    }


    if (newSnakeItem->getX() >= fieldSize || newSnakeItem->getX() < 0 ||
        newSnakeItem->getY() >= fieldSize || newSnakeItem->getY() < 0) {
        GameOver();
        delete newSnakeItem;
        return;
    }
    for (const SnakeItems* item : snakeBody) {
        if (newSnakeItem->getX() == item->getX() && newSnakeItem->getY() == item->getY()) {
            GameOver();
            delete newSnakeItem;
            return;
        }
    }


    if (newSnakeItem->getX() == food->getX() && newSnakeItem->getY() == food->getY()) {
        gameScore++;
        CreateFood();
        QString helpText = "Счёт: " + QString::number(gameScore) + "\nПауза - ПРОБЕЛ";
        emit ChangeTextInHelpField(helpText);
    } else {
        gameSnake->getSnakeBody().removeLast();
    }

    gameSnake->getSnakeBody().insert(0, newSnakeItem);
    repaint();
}



SnakeItems::SnakeItems(int x, int y)
    : snake_x(x), snake_y(y)
{
}


Snake::Snake()
    : m_snakeBeginSize(20), snakeMoveDirection(Snake::right)
{
    for (int i = 0; i < m_snakeBeginSize; ++i) {
        snakeBody.insert(0, new SnakeItems(i, 0));
    }
}

