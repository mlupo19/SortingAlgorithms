#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QPainter>
#include <QtMath>
#include <QApplication>
#include  <cmath>

struct Item {
    int value;
    Qt::GlobalColor color;
};

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QVector<Item>* items, QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void highlight(int index, Qt::GlobalColor color);

public slots:
    void onRefreshRequested();


protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<Item>* items;

signals:
    void refresh();
};

#endif // RENDERAREA_H
