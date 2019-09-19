#include "mainwindow.h"
#include <ui_mainwindow.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();

    // Disenable every pinBox.
    ui->rotateSpinBox->setEnabled(false);
    ui->translateXSpinBox->setEnabled(false);
    ui->translateYSpinBox->setEnabled(false);
    ui->scalarXSpinBox->setEnabled(false);
    ui->scalarYSpinBox->setEnabled(false);


    // Put the root node into the tree_widget.
    connect(ui->mygl, SIGNAL(SendNode()), this, SLOT(slot_addItemToTreeWidget()));

    // Connect pinBox and the node in tree_widget.
    connect(ui->translateXSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_setTX(double)));
    connect(ui->translateYSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_setTY(double)));
    connect(ui->rotateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_setRM(double)));
    connect(ui->scalarXSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_setSX(double)));
    connect(ui->scalarYSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_setSY(double)));

    // Connect create node btn and the according creating function.
    connect(ui->addTranslateNodeBtn, SIGNAL(clicked()), this, SLOT(slot_createT()));
    connect(ui->addRotateNodeBtn, SIGNAL(clicked()), this, SLOT(slot_createR()));
    connect(ui->addScaleNodeBtn, SIGNAL(clicked()), this, SLOT(slot_createS()));

    // Wake up pinBox when a QTreeWidgetItem is selected.
    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem* , int)), this, SLOT(slot_wakeUpPinBox(QTreeWidgetItem* , int)));

    // Connect the geometry set btn and the setGeo()
    connect(ui->setGeoSquareBtn, SIGNAL(clicked()), this, SLOT(slot_setGeo()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::slot_addItemToTreeWidget()
{
    ui->treeWidget->addTopLevelItem(&ui->mygl->GetRoot());
}

// According to the selected item in the widget tree, we wake up the target spin box and set value in the spin box.
void MainWindow::slot_wakeUpPinBox(QTreeWidgetItem* iItem, int iColNum)
{
    Node* result = ui->mygl->TraversalFind(iItem);
    Node* temp = result;
    TranslateNode* t = dynamic_cast<TranslateNode*>(temp);
    RotateNode* r = dynamic_cast<RotateNode*>(temp);
    ScaleNode* s = dynamic_cast<ScaleNode*>(temp);

    if (result != nullptr)
    {
        unsigned int type = result->GetNodeType();
        switch (type)
        {
            case 1: // TranslateNode;
                ui->rotateSpinBox->setEnabled(false);
                ui->translateXSpinBox->setEnabled(true);
                ui->translateYSpinBox->setEnabled(true);
                ui->scalarXSpinBox->setEnabled(false);
                ui->scalarYSpinBox->setEnabled(false);

                // Set current Num;
                if (t != nullptr)
                {
                    ui->translateXSpinBox->setValue(double(t->GetXTranslation()));
                    ui->translateYSpinBox->setValue(double(t->GetYTranslation()));
                }
                break;

            case 2: // RotateNode;
                ui->rotateSpinBox->setEnabled(true);
                ui->translateXSpinBox->setEnabled(false);
                ui->translateYSpinBox->setEnabled(false);
                ui->scalarXSpinBox->setEnabled(false);
                ui->scalarYSpinBox->setEnabled(false);

                // Set current Num;
                if (r != nullptr)
                {
                    ui->rotateSpinBox->setValue(double(r->GetMagnitude()));
                }
                break;

            case 3: // ScaleNode;
                ui->rotateSpinBox->setEnabled(false);
                ui->translateXSpinBox->setEnabled(false);
                ui->translateYSpinBox->setEnabled(false);
                ui->scalarXSpinBox->setEnabled(true);
                ui->scalarYSpinBox->setEnabled(true);

                // Set current Num;
                if (s != nullptr)
                {
                    ui->scalarXSpinBox->setValue(double(s->GetXScalar()));
                    ui->scalarYSpinBox->setValue(double(s->GetYScalar()));
                }
                break;
        }
    }
}


// Set the value in the widget tree node.
void MainWindow::slot_setTX(double value)
{
    QTreeWidgetItem* current = ui->treeWidget->currentItem();
    TranslateNode* t = dynamic_cast<TranslateNode*>(current);
    if (t != nullptr)
    {
        t->SetXTranslation(float(value));
    }
}

void MainWindow::slot_setTY(double value)
{
    QTreeWidgetItem* current = ui->treeWidget->currentItem();
    TranslateNode* t = dynamic_cast<TranslateNode*>(current);
    if (t != nullptr)
    {
        t->SetYTranslation(float(value));
    }
}

void MainWindow::slot_setRM(double value)
{
    QTreeWidgetItem* current = ui->treeWidget->currentItem();
    RotateNode* t = dynamic_cast<RotateNode*>(current);
    if (t != nullptr)
    {
        t->SetMagnitude(float(value));
    }
}

void MainWindow::slot_setSX(double value)
{
    QTreeWidgetItem* current = ui->treeWidget->currentItem();
    ScaleNode* t = dynamic_cast<ScaleNode*>(current);
    if (t != nullptr)
    {
        t->SetXScalar(float(value));
    }
}

void MainWindow::slot_setSY(double value)
{
    QTreeWidgetItem* current = ui->treeWidget->currentItem();
    ScaleNode* t = dynamic_cast<ScaleNode*>(current);
    if (t != nullptr)
    {
        t->SetYScalar(float(value));
    }
}


// Create Node Under the selected node.
void MainWindow::slot_createT()
{
    QTreeWidgetItem* current = ui->treeWidget->currentItem();
    Node* n = dynamic_cast<Node*>(current);
    n->AddChild(std::make_unique<TranslateNode>(0.f, 0.f, QString("New T")));
}

void MainWindow::slot_createR()
{
    QTreeWidgetItem* current = ui->treeWidget->currentItem();
    Node* n = dynamic_cast<Node*>(current);
    n->AddChild(std::make_unique<RotateNode>(0.f, QString("New R")));
}

void MainWindow::slot_createS()
{
    QTreeWidgetItem* current = ui->treeWidget->currentItem();
    Node* n = dynamic_cast<Node*>(current);
    n->AddChild(std::make_unique<ScaleNode>(1.f, 1.f, "New S"));
}

void MainWindow::slot_setGeo()
{
    QTreeWidgetItem* current = ui->treeWidget->currentItem();
    Node* n = dynamic_cast<Node*>(current);
    n->AddGeo(ui->mygl->GetRectangle());
    n->ModifyColor(glm::vec3(0.f, 0.f, 0.f));
}
