#include "help.h"

Help::Help() {
    setFixedSize(400, 100);
    text = "Счёт: 0\nПауза - ПРОБЕЛ";
}

void Help::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    painter.begin(this);
    painter.drawRect(0, 0, width() - 1, height()-1);
    painter.setFont(QFont("Arial", 15, 700));
    painter.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, text);
    painter.end();

}

void Help::ChangeTextInField(QString newText)
{
    text = newText;
    repaint();

}
