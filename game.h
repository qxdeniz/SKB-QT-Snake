#ifndef GAME_H
#define GAME_H
#include <QWidget>


class SnakeItems;
class Snake;
class GameField;

class SnakeItems
{
public:
    SnakeItems(int x, int y);
    int getX() const {return snake_x;}
    int getY() const {return snake_y;}
protected:
    int snake_x;
    int snake_y;
};


class Snake
{
    //friend GameField;

public:
    Snake();
    enum SnakeMoveDirection
    {
        up,
        left,
        down,
        right
    };
    QList<SnakeItems*>& getSnakeBody() { return snakeBody; }
    void setSnakeBody(QList<SnakeItems*>& body) { snakeBody = body; }
    SnakeMoveDirection getMoveDirection() const { return snakeMoveDirection; }
    void setMoveDirection(SnakeMoveDirection direction) { snakeMoveDirection = direction; }
    QList<SnakeItems*> snakeBody;
private:

    int m_snakeBeginSize;

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
     bool crashSnake(QList<SnakeItems*> newSnakeBody);



private slots:
     void MoveSnakeSlot();

signals:
     void ChangeTextInHelpField(QString newText);
};



#endif // GAME_H
