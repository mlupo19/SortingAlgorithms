#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRandomGenerator>
#include <string>

#include "renderarea.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow *ui;
    RenderArea* renderArea;
    QVector<Item>* items;
    void (*sort_items)(QVector<Item>*, RenderArea* renderArea);

private slots:
    void sortButtonClick();
    void on_horizontalSlider_valueChanged(int position);
    void setMergeSort();
    void setBubbleSort();
    void setRadixSort();
    void setQuickSort();
    void setSelectionSort();

private:
    void loadItems();
};
#endif // MAINWINDOW_H
