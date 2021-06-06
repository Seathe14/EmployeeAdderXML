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

void MainWindow::deleteItem(QTreeWidgetItem *itm)
{
    if(departmentItems.contains(itm))
    {
            lastAction = DeleteDepartment;
            undoActions.push(DeleteDepartment);

            int index = ui->treeWidget->indexOfTopLevelItem(itm);
            pb.newDelete(departmentItems.value(itm),index);
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
        undoActions.push(DeleteEmployee);
        lastAction = DeleteEmployee;
        Employee* emplToDelete = dynamic_cast<Employee*>(employeeItems.value(itm));
        int index = itm->parent()->indexOfChild(itm);
        pb.newDelete(emplToDelete,index);
        employeeItems.remove(itm);
        itm->parent()->removeChild(itm);
    }
}

void MainWindow::addExistingItem(DepartmentComponent *itm, int index)
{
    QTreeWidgetItem *child = new QTreeWidgetItem();
    if(dynamic_cast<Departments*>(itm->GetParent())->getName() == "")
    {
        undoActions.push(AddDepartment);

        std::string departmentName = dynamic_cast<Departments*>(itm)->getName();
        child->setText(0,QString::fromStdString(departmentName));
        itm->GetParent()->insert(itm,index);
        ui->treeWidget->insertTopLevelItem(index,child);
        departmentItems.insert(child,itm);
        for(int i = 0; i < itm->numberOfLeaves();i++)
        {
            QTreeWidgetItem *employeeChild = new QTreeWidgetItem();
            std::string FIO = (dynamic_cast<Employee*>(itm->getComponent(i)))->FIO();
            employeeChild->setText(0,QString::fromStdString(FIO));
            employeeItems.insert(employeeChild,itm->getComponent(i));
            child->addChild(employeeChild);
        }
        pb.insertRecord(itm,index);

    }
    else
    {
        undoActions.push(DeleteEmployee);
        std::string FIO = (dynamic_cast<Employee*>(itm))->FIO();
        child->setText(0,QString::fromStdString(FIO));
        std::string parentName = (dynamic_cast<Departments*>(itm->GetParent()))->getName();
        itm->GetParent()->insert(itm,index);
        employeeItems.insert(child,itm);
        departmentItems.key(itm->GetParent())->insertChild(index,child);
        //redoActions.push(DeleteEmployee);
        pb.insertRecord(itm,index);
    }
}



void MainWindow::on_addEmplBtn_clicked()
{
    QTreeWidgetItem *child = new QTreeWidgetItem();
    child->setText(0,ui->surnameLE->text() + " " + ui->nameLE->text() + " " + ui->middleNameLE->text());
    std::string surname = ui->surnameLE->text().toStdString();
    std::string name = ui->nameLE->text().toStdString();
    std::string middleName = ui->middleNameLE->text().toStdString();
    std::string functionInDep = ui->jobLE->text().toStdString();
    std::string salary = ui->salaryLE->text().toStdString();
    Employee* toAdd = new Employee(surname,name,middleName,functionInDep,atoi(salary.c_str()));
    employeeItems.insert(child,toAdd);
    DepartmentComponent* thisDepartment = nullptr;
    if(departmentItems.contains(ui->treeWidget->currentItem()))
    {
        lastAction = AddEmployee;
        ui->treeWidget->currentItem()->addChild(child);
        thisDepartment = departmentItems.value(ui->treeWidget->currentItem());
    }
    else
    {
        lastAction = AddEmployee;
        ui->treeWidget->currentItem()->parent()->addChild(child);
        thisDepartment = departmentItems.value(ui->treeWidget->currentItem()->parent());
    }
    //ui->treeWidget->currentItem()->in
    undoActions.push(AddEmployee);
    thisDepartment->add(toAdd);
    pb.newAdd(toAdd);
}

void MainWindow::fillItems()
{
    for(int i =0;i<pb.departments.size();i++)
    {
        QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
        itm->setText(0,QString::fromStdString(dynamic_cast<Departments*>(pb.departmentss->getComponent(i))->getName()));
        departmentItems.insert(itm,pb.departmentss->getComponent(i));
        departments.insert(itm,QString::fromStdString(pb.departments[i].getName()));
        ui->treeWidget->insertTopLevelItem(0,itm);
    }
    for(int i =0;i<pb.departmentss->numberOfLeaves();i++)
    {
        for(int j =0;j<pb.departmentss->getComponent(i)->numberOfLeaves();j++)
        {
            std::string FIO = dynamic_cast<Employee*>(pb.departmentss->getComponent(i)->getComponent(j))->FIO();
            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setText(0,QString::fromStdString(FIO));
            employeeItems.insert(child,pb.departmentss->getComponent(i)->getComponent(j));
            for(auto k : departmentItems.keys())
            {
                if(k->text(0).toStdString() == dynamic_cast<Departments*>(pb.departmentss->getComponent(i))->getName())
                    k->addChild(child);
            }
        }
    }
}

void MainWindow::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column)
{
    if(item->parent() == nullptr)
        return;
    Employee* employeeItm = dynamic_cast<Employee*>(employeeItems.value(item));
    ui->surnameLE->setText(QString::fromStdString(employeeItm->getSurname()));
    ui->nameLE->setText(QString::fromStdString(employeeItm->getName()));
    ui->middleNameLE->setText(QString::fromStdString(employeeItm->getMiddleName()));
    ui->jobLE->setText(QString::fromStdString(employeeItm->getFunctionInDep()));
    ui->salaryLE->setText(QString::fromStdString(std::to_string(employeeItm->getSalary())));
}

void MainWindow::on_delEmplBtn_clicked()
{
    QTreeWidgetItem *itm  = ui->treeWidget->currentItem();
    deleteItem(itm);
}

void MainWindow::on_addDepBtn_clicked()
{
    lastAction = AddDepartment;
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    itm->setText(0,ui->newDepartmentLE->text());
    ui->treeWidget->insertTopLevelItem(0,itm);
    std::string departmentName = ui->newDepartmentLE->text().toStdString();
    DepartmentComponent* newDepartment = new Departments(departmentName);
    departmentItems.insert(itm,newDepartment);
    newDepartment->setParent(nullptr);
    pb.newAdd(newDepartment);

}

void MainWindow::on_undoBtn_clicked()
{

    QTreeWidgetItem *child = new QTreeWidgetItem();
    lastAction = undoActions.top();
    std::pair<DepartmentComponent*,int> undoItem = pb.getUndoTopItem();
    undoActions.pop();
    if(lastAction == DeleteEmployee)
    {
        addExistingItem(undoItem.first,undoItem.second);
        //std::string FIO = (dynamic_cast<Employee*>(undoItem.first))->FIO();
        //child->setText(0,QString::fromStdString(FIO));
        //std::string parentName = (dynamic_cast<Departments*>(undoItem.first->GetParent()))->getName();
        //undoItem.first->GetParent()->insert(undoItem.first,undoItem.second);
        //employeeItems.insert(child,undoItem.first);
        //departmentItems.key(undoItem.first->GetParent())->insertChild(undoItem.second,child);
        redoActions.push(DeleteEmployee);
        //pb.insertRecord(undoItem.first,undoItem.second);
    }
    else if(lastAction == DeleteDepartment)
    {
        addExistingItem(undoItem.first,undoItem.second);
        //std::string departmentName = dynamic_cast<Departments*>(undoItem.first)->getName();
        //child->setText(0,QString::fromStdString(departmentName));
        //undoItem.first->GetParent()->insert(undoItem.first,undoItem.second);
        //ui->treeWidget->insertTopLevelItem(undoItem.second,child);
        //departmentItems.insert(child,undoItem.first);
        //for(int i = 0; i < undoItem.first->numberOfLeaves();i++)
        //{
        //    QTreeWidgetItem *employeeChild = new QTreeWidgetItem();
        //    std::string FIO = (dynamic_cast<Employee*>(undoItem.first->getComponent(i)))->FIO();
        //    employeeChild->setText(0,QString::fromStdString(FIO));
        //    employeeItems.insert(employeeChild,undoItem.first->getComponent(i));
        //    child->addChild(employeeChild);
        //}
        redoActions.push(DeleteDepartment);
        //pb.insertRecord(undoItem.first,undoItem.second);
    }
    else if(lastAction == AddEmployee)
    {
        for(auto i = employeeItems.begin();i!=employeeItems.end();i++)
        {
            if(i.value() == undoItem.first)
            {
                deleteItem(i.key());
                break;
            }
        }
        redoActions.push(AddEmployee);
    }
    else if(lastAction == AddDepartment)
    {
        for(auto i = departmentItems.begin();i!=departmentItems.end();i++)
        {
            if(i.value() == undoItem.first)
            {
                deleteItem(i.key());
                break;
            }
        }
    }
}

void MainWindow::on_redoBtn_clicked()
{
    std::pair<DepartmentComponent*,int> redoItem = pb.getRedoTopItem();
    actions toPerformRedo = redoActions.top();
    redoActions.pop();
    if(toPerformRedo == DeleteEmployee)
    {
        for(auto i = employeeItems.begin();i!=employeeItems.end();i++)
        {
            if(i.value() == redoItem.first)
            {
                deleteItem(i.key());
                break;
            }
        }
    }
    else if(toPerformRedo == DeleteDepartment)
    {
        for(auto i = departmentItems.begin();i!=departmentItems.end();i++)
        {
            if(i.value() == redoItem.first)
            {
                deleteItem(i.key());
            }
        }
    }
    else if(toPerformRedo == AddEmployee)
    {
        addExistingItem(redoItem.first,redoItem.second);
    }
    else if(toPerformRedo == AddDepartment)
    {

    }
}
