#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMap>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::deleteItem(QTreeWidgetItem *itm)
{
    if(departmentItems.contains(itm))
    {
        int index = ui->treeWidget->indexOfTopLevelItem(itm);
        pb.deleteRecord(departmentItems.value(itm),index,TODELETEDEPARTMENT);
        for(auto i : employeeItems.keys())
        {
            if(i->parent() == itm)
            {
                employeeItems.remove(i);
                itm->removeChild(i);
            }
        }
        departmentItems.remove(itm);
        delete ui->treeWidget->takeTopLevelItem(index);
    }
    else
    {
        Employee* emplToDelete = dynamic_cast<Employee*>(employeeItems.value(itm));
        int index = itm->parent()->indexOfChild(itm);
        pb.deleteRecord(emplToDelete,index,TODELETEEMPLOYEE);
        employeeItems.remove(itm);
        itm->parent()->removeChild(itm);
        delete itm;
    }
}

void MainWindow::addEmployeeItem()
{
    if(ui->treeWidget->currentItem() == nullptr)
    {
        QMessageBox msgBox;
        msgBox.setText("Please, choose department where to add");
        msgBox.setWindowTitle("Error");
        msgBox.exec();
        return;
    }
    QTreeWidgetItem *child = new QTreeWidgetItem();
    child->setText(0,ui->surnameLE->text() + " " + ui->nameLE->text() + " " + ui->middleNameLE->text());
    std::string surname = ui->surnameLE->text().toStdString();
    std::string name = ui->nameLE->text().toStdString();
    std::string middleName = ui->middleNameLE->text().toStdString();
    std::string functionInDep = ui->jobLE->text().toStdString();
    std::string salary = ui->salaryLE->text().toStdString();

    //if(ui->salaryLE->text.toInt()
    if(surname == "" || name == "" || functionInDep == "" || salary == "")
    {
        QMessageBox msgBox;
        msgBox.setText("Please, type in employee info");
        msgBox.setWindowTitle("Error");
        msgBox.exec();
        return;
    }
    bool ok;
    int result = ui->salaryLE->text().toInt(&ok,10);
    if(!ok)
    {
        QMessageBox msgBox;
        msgBox.setText("Salary is integer");
        msgBox.setWindowTitle("Error");
        msgBox.exec();
        return;
    }
    //if(salary.toInt)
    Employee* toAdd = new Employee(surname,name,middleName,functionInDep,atoi(salary.c_str()));
    employeeItems.insert(child,toAdd);
    DepartmentComponent* thisDepartment = nullptr;
    if(departmentItems.contains(ui->treeWidget->currentItem()))
    {
        ui->treeWidget->currentItem()->addChild(child);
        thisDepartment = departmentItems.value(ui->treeWidget->currentItem());
    }
    else
    {
        ui->treeWidget->currentItem()->parent()->addChild(child);
        thisDepartment = departmentItems.value(ui->treeWidget->currentItem()->parent());
    }
    toAdd->setParent(thisDepartment);
    pb.addRecord(toAdd);
}

void MainWindow::on_addEmplBtn_clicked()
{
    //if(ui->treeWidget->currentItem() != nullptr)
     addEmployeeItem();
}

void MainWindow::fillItems()
{
    for(int i =0;i<dynamic_cast<Departments*>(pb.departments)->numberOfLeaves();i++)
    {
        QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
        itm->setText(0,QString::fromStdString(dynamic_cast<Departments*>(pb.departments->getComponent(i))->getName()));
        departmentItems.insert(itm,pb.departments->getComponent(i));
        ui->treeWidget->insertTopLevelItem(0,itm);
    }
    for(int i =0;i<dynamic_cast<Departments*>(pb.departments)->numberOfLeaves();i++)
    {
        for(int j =0;j<dynamic_cast<Departments*>(pb.departments->getComponent(i))->numberOfLeaves();j++)
        {
            std::string FIO = dynamic_cast<Employee*>(pb.departments->getComponent(i)->getComponent(j))->FIO();
            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setText(0,QString::fromStdString(FIO));
            employeeItems.insert(child,pb.departments->getComponent(i)->getComponent(j));
            for(auto departmentItem : departmentItems.keys())
            {
                if(departmentItem->text(0).toStdString() == dynamic_cast<Departments*>(pb.departments->getComponent(i))->getName())
                    departmentItem->addChild(child);
                departmentItem->setExpanded(true);
            }
        }
    }
}

void MainWindow::clearItems()
{
    departmentItems.clear();
    employeeItems.clear();
    int count = ui->treeWidget->topLevelItemCount();
    for(int i =0;i<count;i++)
    {
        for(int j =0;j<ui->treeWidget->topLevelItem(0)->childCount();j++)
        {
            QTreeWidgetItem* child = ui->treeWidget->topLevelItem(0)->child(j);
            ui->treeWidget->topLevelItem(0)->removeChild(child);
            delete child;
        }
        QTreeWidgetItem* toplevelItem = ui->treeWidget->takeTopLevelItem(0);
        ui->treeWidget->removeItemWidget(toplevelItem,0);
        delete toplevelItem;
    }
    ui->treeWidget->clear();
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if(item->parent() == nullptr)
    {
        ui->departmentAvgSalaryLE->setText(QString::number(dynamic_cast<Departments*>(departmentItems.value(item))->countAvgSalary()));
        ui->departmentEmployeeNumLE ->setText(QString::number(dynamic_cast<Departments*>(departmentItems.value(item))->getNumOfEmployees()));
        return;
    }
}
void MainWindow::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column)
{
    if(item->parent() == nullptr)
    {
        return;
    }
    Employee* employeeItm = dynamic_cast<Employee*>(employeeItems.value(item));
    //employeeItm->add();
    ui->surnameLE->setText(QString::fromStdString(employeeItm->getSurname()));
    ui->nameLE->setText(QString::fromStdString(employeeItm->getName()));
    ui->middleNameLE->setText(QString::fromStdString(employeeItm->getMiddleName()));
    ui->jobLE->setText(QString::fromStdString(employeeItm->getFunctionInDep()));
    ui->salaryLE->setText(QString::fromStdString(std::to_string(employeeItm->getSalary())));
}

void MainWindow::on_delEmplBtn_clicked()
{
    if(ui->treeWidget->currentItem() == nullptr)
    {
        QMessageBox msgBox;
        msgBox.setText("Please choose item to delete");
        msgBox.setWindowTitle("Error");
        msgBox.exec();
        return;
    }
    QTreeWidgetItem *itm  = ui->treeWidget->currentItem();
    deleteItem(itm);
}

void MainWindow::on_addDepBtn_clicked()
{
    if(ui->newDepartmentLE->text() != "")
    {
        QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
        itm->setText(0,ui->newDepartmentLE->text());
        ui->treeWidget->insertTopLevelItem(0,itm);
        std::string departmentName = ui->newDepartmentLE->text().toStdString();
        DepartmentComponent* newDepartment = new Departments(departmentName);
        departmentItems.insert(itm,newDepartment);
        newDepartment->setParent(nullptr);
        pb.addRecord(newDepartment);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Please, type in department name");
        msgBox.setWindowTitle("Error");
        msgBox.exec();
    }
}

void MainWindow::on_undoBtn_clicked()
{
    pb.undo();
    clearItems();
    fillItems();
}

void MainWindow::on_redoBtn_clicked()
{
    pb.redo();
    clearItems();
    fillItems();
}



void MainWindow::on_saveBtn_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save file"),"default",tr("xml (*.xml)"));
    pb.saveFile(fileName.toStdString());
}

void MainWindow::on_loadBtn_clicked()
{
    clearItems();
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"),"tst.xml",tr("xml (*.xml)"));
    pb.loadFile(fileName.toStdString());
    fillItems();
}
