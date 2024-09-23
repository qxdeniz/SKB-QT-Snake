#ifndef HELP_H
#define HELP_H
#include <QWidget>
#include <QPainter>


class Help: public QWidget
{
    Q_OBJECT
public:
    Help();

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    QString text;

public slots:
    void ChangeTextInField(QString newText);
};

#endif // HELP_H
