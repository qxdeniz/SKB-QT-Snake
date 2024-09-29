#include "game.h"
#include <QPainter>
#include <QList>
#include <QTimer>
#include <QKeyEvent>
#include <QRandomGenerator>

GameField::GameField()
{
    setFixedSize(400, 400);
    setFocusPolicy(Qt::StrongFocus);
    snakeItemSize = 10;
    fieldSize = width()/snakeItemSize;
    StartNewGame();
}

void GameField::paintEvent(QPaintEvent *e)
{
    QBrush snakeColor(QColor(102, 255, 0));
    QBrush foodColor(QColor(255, 0, 0));


    QPainter painter;
    painter.begin(this);

    if (gameStatus){
        painter.setFont(QFont("Arial", 30, 700));
        painter.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, "ВЫ ПРОИГРАЛИ!\nВаш счёт: " + QString::number(gameScore));
        return;
    }

    painter.drawRect(0, 0, width() - 1, height()-1);

    painter.setBrush(snakeColor);

    for (int i = 0; i < gameSnake -> snakeBody.size(); i++){
        painter.drawEllipse(gameSnake -> snakeBody[i] -> snake_x * snakeItemSize, gameSnake -> snakeBody[i] -> snake_y * snakeItemSize, snakeItemSize, snakeItemSize);
    }

    painter.setBrush(foodColor);

    painter.drawEllipse(food -> snake_x * snakeItemSize, food -> snake_y * snakeItemSize, snakeItemSize, snakeItemSize);
    painter.end();


}

void GameField::keyPressEvent(QKeyEvent *e)
{
    Snake::SnakeMoveDirection currentDirection = gameSnake->snakeMoveDirection;

    if ((e->key() == Qt::Key_Right || e->key() == Qt::Key_D) && currentDirection != Snake::SnakeMoveDirection::left){
        gameSnake->snakeMoveDirection = Snake::SnakeMoveDirection::right;
    }
    else if ((e->key() == Qt::Key_Left || e->key() == Qt::Key_A) && currentDirection != Snake::SnakeMoveDirection::right){
        gameSnake->snakeMoveDirection = Snake::SnakeMoveDirection::left;
    }
    else if ((e->key() == Qt::Key_Down || e->key() == Qt::Key_S) && currentDirection != Snake::SnakeMoveDirection::up){
        gameSnake->snakeMoveDirection = Snake::SnakeMoveDirection::down;
    }
    else if ((e->key() == Qt::Key_Up || e->key() == Qt::Key_W) && currentDirection != Snake::SnakeMoveDirection::down){
        gameSnake->snakeMoveDirection = Snake::SnakeMoveDirection::up;
    }
    else if (e->key() == Qt::Key_Space){
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
    gameStatus = false;
    isGameOnPause = false;
    connect(moveSnakeTimer, &QTimer::timeout, this, &GameField::MoveSnakeSlot);
    snakeMoves.append("right");
    moveSnakeTimer -> start(100);

}

void GameField::CreateFood()
{
    food -> snake_x = QRandomGenerator::global() -> bounded(0, fieldSize - 1);
    food -> snake_y = QRandomGenerator::global() -> bounded(0, fieldSize - 1);

    for (int i = 0; i < gameSnake -> snakeBody.size(); i++){
        if (food -> snake_x == gameSnake -> snakeBody[i] -> snake_x && food -> snake_y == gameSnake -> snakeBody[i] -> snake_y){
            return CreateFood();
        }
    }



}

void GameField::MoveSnakeSlot()
{
    SnakeItems *newSnakeItem = nullptr;
    Snake::SnakeMoveDirection currentDirection = gameSnake->snakeMoveDirection;

    if (currentDirection == Snake::SnakeMoveDirection::right){
        newSnakeItem = new SnakeItems(gameSnake->snakeBody[0]->snake_x + 1, gameSnake->snakeBody[0]->snake_y);
    }
    else if (currentDirection == Snake::SnakeMoveDirection::left){
        newSnakeItem = new SnakeItems(gameSnake->snakeBody[0]->snake_x - 1, gameSnake->snakeBody[0]->snake_y);
    }
    else if (currentDirection == Snake::SnakeMoveDirection::up){
        newSnakeItem = new SnakeItems(gameSnake->snakeBody[0]->snake_x, gameSnake->snakeBody[0]->snake_y - 1);
    }
    else if (currentDirection == Snake::SnakeMoveDirection::down){
        newSnakeItem = new SnakeItems(gameSnake->snakeBody[0]->snake_x, gameSnake->snakeBody[0]->snake_y + 1);
    }


    if (newSnakeItem->snake_x >= fieldSize || newSnakeItem->snake_x < 0 ||
        newSnakeItem->snake_y >= fieldSize || newSnakeItem->snake_y < 0){
        GameOver();
    }
    else {
        for (int i = 0; i < gameSnake->snakeBody.size(); i++){
            if (newSnakeItem->snake_x == gameSnake->snakeBody[i]->snake_x && newSnakeItem->snake_y == gameSnake->snakeBody[i]->snake_y){
                GameOver();
            }
        }
    }

    if (newSnakeItem->snake_x == food->snake_x && newSnakeItem->snake_y == food->snake_y){
        gameScore++;
        CreateFood();
        QString helpText = "Счёт: " + QString::number(gameScore) + "\nПауза - ПРОБЕЛ";
        emit ChangeTextInHelpField(helpText);
    } else {
        gameSnake->snakeBody.removeLast();
    }

    gameSnake->snakeBody.insert(0, newSnakeItem);

    repaint();
}




SnakeItems::SnakeItems(int x, int y)
{
    snake_x = x;
    snake_y = y;

}

Snake::Snake()
{
    m_snakeBeginSize = 2;
    for (int i = 0; i < m_snakeBeginSize; i++)
    {
        snakeBody.insert(0, new SnakeItems(i, 0));
    }

    snakeMoveDirection = SnakeMoveDirection::right;



}
