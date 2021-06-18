#include "renderarea.h"

#include <QDebug>

RenderArea::RenderArea(QVector<Item>* items, QWidget *parent) : QWidget(parent)
{
    this->items = items;

    connect(this, SIGNAL(refresh()), this, SLOT(onRefreshRequested()));
}

void RenderArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    double size, space = 1, scale = 0.5;

    size = this->width() / (items->size() * (1.0 + space));

    int numItems = items->size();
    for (int i = 0; i < numItems; i++) {
        QRect rect(static_cast<int>(i * (size + space)), 625 - static_cast<int>(items->at(i).value * scale), size, items->at(i).value * scale);
        painter.setPen(items->at(i).color);
        painter.fillRect(rect, Qt::black);
    }
}

QSize RenderArea::sizeHint() const
{
    return QSize(600, 200);
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(400, 200);
}

void RenderArea::highlight(int index, Qt::GlobalColor color) {
    (*items)[index].color = color;
    update();
}

void RenderArea::onRefreshRequested() {
    update();
//    qApp->processEvents();
}
