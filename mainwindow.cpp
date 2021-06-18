#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>

void mergeSort(QVector<Item>* items, RenderArea* renderArea);
void bubbleSort(QVector<Item>* items, RenderArea* renderArea);
void radixSort(QVector<Item>* items, RenderArea* renderArea);
void quickSort(QVector<Item>* items, RenderArea* renderArea);
void selectionSort(QVector<Item>* items, RenderArea* renderArea);
void threadRun(MainWindow* mw);

bool running = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    sort_items = mergeSort;

    items = new QVector<Item>;
    loadItems();
    renderArea = new RenderArea(items);

    connect(ui->sortButton, SIGNAL(clicked()), this, SLOT(sortButtonClick()));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui->actionMerge_Sort, SIGNAL(triggered()), this, SLOT(setMergeSort()));
    connect(ui->actionBubble_Sort, SIGNAL(triggered()), this, SLOT(setBubbleSort()));
    connect(ui->actionQuick_Sort, SIGNAL(triggered()), this, SLOT(setQuickSort()));
    connect(ui->actionRadix_Sort, SIGNAL(triggered()), this, SLOT(setRadixSort()));
    connect(ui->actionSelection_Sort, SIGNAL(triggered()), this, SLOT(setSelectionSort()));

    ui->gridLayout->addWidget(renderArea, 0, 0, 1, 1);
    ui->labelNum->setText(QString::number(ui->horizontalSlider->value()));
}

MainWindow::~MainWindow() {
    delete ui;
    delete renderArea;
    delete items;
}

void MainWindow::sortButtonClick() {
    if (!running) {
        QThread* sortThread = QThread::create(&threadRun, this);
        sortThread->start();
        update();
    }
}

void MainWindow::on_horizontalSlider_valueChanged(int position) {
    ui->labelNum->setText(QString::number(ui->horizontalSlider->value()));
    loadItems();
    update();
}

void MainWindow::setMergeSort() {
    sort_items = mergeSort;
    ui->sortButton->setText(tr("Merge Sort"));
    loadItems();
    update();
}

void MainWindow::setBubbleSort() {
    sort_items = bubbleSort;
    ui->sortButton->setText(tr("Bubble Sort"));
    loadItems();
    update();
}

void MainWindow::setRadixSort() {
    sort_items = radixSort;
    ui->sortButton->setText(tr("Radix Sort"));
    loadItems();
    update();
}

void MainWindow::setQuickSort() {
    sort_items = quickSort;
    ui->sortButton->setText(tr("Quick Sort"));
    loadItems();
    update();
}

void MainWindow::setSelectionSort() {
    sort_items = selectionSort;
    ui->sortButton->setText(tr("Selection Sort"));
    loadItems();
    update();
}


void MainWindow::loadItems() {
    if (!running) {
        int num = ui->horizontalSlider->value();
        items->clear();
        for (int i = 0; i < num; i++) {
            Item item = {QRandomGenerator::global()->bounded(1000), Qt::black};
            items->append(item);
        }
    }
}

void threadRun(MainWindow* mw) {
    if (running)
       return;

    running = true;
    mw->sort_items(mw->items, mw->renderArea);
    running  = false;
}

void merge(QVector<Item>& items, int l, int m, int r, RenderArea* renderArea) {
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temp arrays
    Item L[n1], R[n2];

    // Copy data to temp arrays L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = items[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = items[m + 1 + j];

    // Merge the temp arrays back into arr[l..r]

    // Initial index of first subarray
    int i = 0;

    // Initial index of second subarray
    int j = 0;

    // Initial index of merged subarray
    int k = l;

    while (i < n1 && j < n2) {
        if (L[i].value <= R[j].value) {
            items[k] = L[i];
            i++;
        }
        else {
            items[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of
    // L[], if there are any
    while (i < n1) {
        items[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of
    // R[], if there are any
    while (j < n2) {
        items[k] = R[j];
        j++;
        k++;
    }
}

void mergeHelper(QVector<Item>& items, int l, int r, RenderArea* renderArea) {
    if (r > l) {
        int m = l+ (r-l)/2;

        mergeHelper(items, l, m, renderArea);
        mergeHelper(items, m+1, r, renderArea);
        emit renderArea->refresh();
        QThread::usleep(7000);
        merge(items, l, m, r, renderArea);
        emit renderArea->refresh();
        QThread::usleep(3000);
    }
}

void mergeSort(QVector<Item>* items_pointer, RenderArea* renderArea) {
    QVector<Item>& items = *items_pointer;
    mergeHelper(items, 0, items.size()-1, renderArea);
    emit renderArea->refresh();
}

void bubbleSort(QVector<Item>* items_pointer, RenderArea* renderArea) {
    QVector<Item>& items = *items_pointer;
    for (int i = 0; i < items.size(); i++) {
        for (int j = 1; j < items.size() - i; j++) {
            if (items[j-1].value > items[j].value) {
                int temp = items[j].value;
                items[j] = items[j-1];
                items[j-1].value = temp;
                emit renderArea->refresh();
            }
        }
        emit renderArea->refresh();
        QThread::msleep(32);
    }
}

void countSort(QVector<Item>* items, int n, int exp, RenderArea* renderArea) {
    Item output[n];
    int i, count[10] = {0};

    for (int i = 0; i < n; i++) {
        count[((*items)[i].value/exp) % 10]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n-1; i >= 0; i--) {
        output[count[((*items)[i].value / exp) % 10] - 1] = (*items)[i];
        count[((*items)[i].value / exp) % 10]--;
    }

    for (int i = 0; i < n; i++) {
        (*items)[i] = output[i];
        emit renderArea->refresh();
        QThread::usleep(2400);
    }
}

void radixSort(QVector<Item>* items, RenderArea* renderArea) {
    QVector<Item>& itemsRef = *items;
    Item mx = itemsRef[0];
    int numItems = itemsRef.size();
    for (int i = 1; i < numItems; i++)
        if (itemsRef[i].value > mx.value)
            mx = itemsRef[i];
    for (int exp = 1; mx.value / exp > 0; exp*=10) {
        countSort(items, numItems, exp, renderArea);
    }
}

void quickSortHelper(QVector<Item>* items, RenderArea* renderArea, int low, int high) {
    if (low < high) {
        int pivot = (*items)[high].value;

        int i = low - 1;

        for (int j = low; j <= high - 1; j++) {
            if ((*items)[j].value < pivot) {
                i++;
                Item temp = (*items)[j];
                (*items)[j] = (*items)[i];
                (*items)[i] = temp;
                emit renderArea->refresh();
                QThread::usleep(1);
            }
        }



        Item temp = (*items)[i + 1];
        (*items)[i + 1] = (*items)[high];
        (*items)[high] = temp;

        i++;

        quickSortHelper(items, renderArea, low, i - 1);
        quickSortHelper(items, renderArea, i + 1, high);
        emit renderArea->refresh();
    }
}

void quickSort(QVector<Item>* items, RenderArea* renderArea) {
    quickSortHelper(items, renderArea, 0, items->size()-1);
}

void selectionSort(QVector<Item>* items, RenderArea* renderArea) {
    for (int i = 0; i < items->size(); i++) {
        Item* min = &(*items)[i];
        int index = i;
        for (int j = i; j < items->size(); j++) {
            if ((*items)[j].value < min->value) {
                min = &(*items)[j];
                index = j;
            }
        }
        QThread::msleep(24);
        Item temp = *min;
        (*items)[index] = (*items)[i];
        (*items)[i] = temp;
        emit renderArea->refresh();
    }
}
