#ifndef GAME_H
#define GAME_H
#include <QWidget>


class SnakeItems;
class Snake;
class GameField;

class SnakeItems
{
    friend Snake;
    friend GameField;
public:
    SnakeItems(int x, int y);
private:
    int snake_x;
    int snake_y;
};


class Snake
{
    friend GameField;
public:
    Snake();
private:
    QList<SnakeItems*> snakeBody;
    int m_snakeBeginSize;
    enum SnakeMoveDirection
    {
        up,
        left,
        down,
        right
    };
    SnakeMoveDirection snakeMoveDirection;


};

class GameField: public QWidget
{
     Q_OBJECT
public:
     GameField();

protected:
    void paintEvent(QPaintEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
private:
     Snake *gameSnake;
     SnakeItems *food;
     int snakeItemSize;
     QTimer *moveSnakeTimer;
     int fieldSize;
     bool isGameOnPause;
     void setGameOnPause(bool isGameOnPause);
     int gameScore;
     bool gameStatus;
     void GameOver();
     void StartNewGame();
     void CreateFood();
     bool crashSnake(const QList<SnakeItems*> newSnakeBody);
     QList<QString> snakeMoves;


private slots:
     void MoveSnakeSlot();

signals:
     void ChangeTextInHelpField(QString newText);
};



#endif // GAME_H
