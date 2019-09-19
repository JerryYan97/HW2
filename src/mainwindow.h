#pragma once

#include <QMainWindow>
#include "mygl.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();

private:
    Ui::MainWindow *ui;

public slots:
    void slot_addItemToTreeWidget();
    void slot_wakeUpPinBox(QTreeWidgetItem* iItem, int iColNum);

    // Set the value in the widget tree node and the tree.
    void slot_setTX(double value);
    void slot_setTY(double value);
    void slot_setRM(double value);
    void slot_setSX(double value);
    void slot_setSY(double value);

    // Create Node Under the selected node.
    void slot_createT();
    void slot_createR();
    void slot_createS();

    // Create Geometry for a selected node.
    void slot_setGeo();
};
