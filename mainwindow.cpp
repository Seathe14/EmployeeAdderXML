#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMap>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fillItems();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_addEmplBtn_clicked()
{

    QTreeWidgetItem *child = new QTreeWidgetItem();
    child->setText(0,ui->surnameLE->text() + " " + ui->nameLE->text() + " " + ui->middleNameLE->text());
    if(departments.contains(ui->treeWidget->currentItem()))
    {
        ui->treeWidget->currentItem()->addChild(child);
        employee NewEmply(ui->surnameLE->text().toStdString(),ui->nameLE->text().toStdString(),ui->middleNameLE->text().toStdString(),
                          ui->jobLE->text().toStdString(),ui->salaryLE->text().toStdString());
        employeeChildren.insert(child,NewEmply);
        for(int i =0;i<pb.departments.size();i++)
        {
            if(pb.departments[i].getName() == ui->treeWidget->currentItem()->text(0).toStdString())
            {
                pb.departments[i].addEmployee(NewEmply);
                pb.addRecord(pb.departments[i].getName(),NewEmply);
            }
        }
    }
    else
    {
        ui->treeWidget->currentItem()->parent()->addChild(child);
        employee NewEmply(ui->surnameLE->text().toStdString(),ui->nameLE->text().toStdString(),ui->middleNameLE->text().toStdString(),
                          ui->jobLE->text().toStdString(),ui->salaryLE->text().toStdString());
        employeeChildren.insert(child,NewEmply);
        for(int i =0;i<pb.departments.size();i++)
        {
            if(pb.departments[i].getName() == ui->treeWidget->currentItem()->parent()->text(0).toStdString())
            {
                pb.departments[i].addEmployee(NewEmply);
                pb.addRecord(pb.departments[i].getName(),NewEmply);
            }
        }
    }
}

void MainWindow::fillItems()
{
    for(int i =0;i<pb.departments.size();i++)
    {
        QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
        //itm->setText(0, QString::fromStdString(pb.departments[i].getName()));
        itm->setText(0,QString::fromStdString(dynamic_cast<Departments*>(pb.departmentss->getComponent(i))->getName()));
        departments.insert(itm,QString::fromStdString(pb.departments[i].getName()));
        ui->treeWidget->insertTopLevelItem(0,itm);
    }

    for(int j =0;j<pb.departments.size();j++)
    for(int i =0;i<pb.departments[j].getEmployeeNum();i++)
    {
        std::string FIO = pb.departments[j].employees[i].getSurname() + " " + pb.departments[j].employees[i].getName() + " " + pb.departments[j].employees[i].getMiddleName();
        QTreeWidgetItem *child = new QTreeWidgetItem();
        child->setText(0,QString::fromStdString(FIO));
        employeeChildren.insert(child,pb.departments[j].employees[i]);
        for(auto k : departments.keys())
        {
            if(k->text(0).toStdString() == pb.departments[j].getName())
                k->addChild(child);
        }
        //departments[j]->addChild(child);
    }
}

void MainWindow::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column)
{
    if(item->parent() == nullptr)
        return;
    employee selectedEmployee;
    selectedEmployee = employeeChildren.value(item);
    ui->surnameLE->setText(QString::fromStdString(selectedEmployee.getSurname()));
    ui->nameLE->setText(QString::fromStdString(selectedEmployee.getName()));
    ui->middleNameLE->setText(QString::fromStdString(selectedEmployee.getMiddleName()));
    ui->jobLE->setText(QString::fromStdString(selectedEmployee.getFunction()));
    ui->salaryLE->setText(QString::fromStdString(selectedEmployee.getSalary()));
}

void MainWindow::on_delEmplBtn_clicked()
{
    QTreeWidgetItem *itm  = ui->treeWidget->currentItem();
    if(departments.contains(ui->treeWidget->currentItem()))
    {
        pb.deleteDepartment(ui->treeWidget->currentItem()->text(0).toStdString());
        for(auto i = employeeChildren.begin();i!=employeeChildren.end();i++)
        {
            itm->removeChild(i.key());
        }
        //ui->treeWidget->removeItemWidget(itm,0);
        int x = ui->treeWidget->indexOfTopLevelItem(itm);
        delete ui->treeWidget->takeTopLevelItem(x);
    }
    else
    {
    employee employeeToDelete = employeeChildren.value(itm);
    pb.deleteRecord(itm->parent()->text(0).toStdString(),employeeToDelete);
    itm->parent()->removeChild(itm);
    }
    //itm->parent();
}
