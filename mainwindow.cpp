#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMap>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(checkUndoRedoButtons()));
    timer->start(35);
    fillItems();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::deleteItem(QTreeWidgetItem *itm,bool toPush)
{
    if(departmentItems.contains(itm))
    {
            if(toPush)
                undoActions.push(AddDepartment);
            //if(pb.peekRedoItem().first->GetParent() == departmentItems.value(itm))
            //{
            //    std::pair<DepartmentComponent*,int> undoItem = pb.getUndoTopItem(TODELETE);
            //    addExistingItem(undoItem.first,undoItem.second,false);
            //    //redoActions.push(DeleteEmployee);
            //}
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
        if(toPush)
            undoActions.push(AddEmployee);
        Employee* emplToDelete = dynamic_cast<Employee*>(employeeItems.value(itm));
        int index = itm->parent()->indexOfChild(itm);
        pb.newDelete(emplToDelete,index);
        employeeItems.remove(itm);
        itm->parent()->removeChild(itm);
    }
}

void MainWindow::addExistingItem(DepartmentComponent *itm, int index,bool toPush)
{
    QTreeWidgetItem *child = new QTreeWidgetItem();
    if(dynamic_cast<Departments*>(itm->GetParent())->getName() == "")
    {
        if(toPush)
            undoActions.push(DeleteDepartment);
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
        if(toPush)
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

void MainWindow::checkUndoRedoButtons()
{
    if(pb.isUndoEmpty())
        ui->undoBtn->setEnabled(0);
    else
        ui->undoBtn->setEnabled(1);
    if(pb.isRedoEmpty())
        ui->redoBtn->setEnabled(0);
    else
        ui->redoBtn->setEnabled(1);
}



void MainWindow::on_addEmplBtn_clicked()
{
    pb.setToPush(true);
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
        ui->treeWidget->currentItem()->addChild(child);
        thisDepartment = departmentItems.value(ui->treeWidget->currentItem());
    }
    else
    {
        ui->treeWidget->currentItem()->parent()->addChild(child);
        thisDepartment = departmentItems.value(ui->treeWidget->currentItem()->parent());
    }
    //ui->treeWidget->currentItem()->in
    undoActions.push(DeleteEmployee);
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
    pb.setToPush(true);
    QTreeWidgetItem *itm  = ui->treeWidget->currentItem();
    deleteItem(itm,true);
}

void MainWindow::on_addDepBtn_clicked()
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    itm->setText(0,ui->newDepartmentLE->text());
    ui->treeWidget->insertTopLevelItem(0,itm);
    std::string departmentName = ui->newDepartmentLE->text().toStdString();
    DepartmentComponent* newDepartment = new Departments(departmentName);
    departmentItems.insert(itm,newDepartment);
    newDepartment->setParent(nullptr);
    undoActions.push(DeleteDepartment);
    pb.newAdd(newDepartment);

}

void MainWindow::on_undoBtn_clicked()
{
    actions toPerformUndo = undoActions.top();
    //lastAction = undoActions.top();
    //undoActions.pop();
    undoActions.pop();

    if(toPerformUndo == AddEmployee)
    {
        std::pair<DepartmentComponent*,int> undoItem = pb.getUndoTopItem(TODELETE);
        addExistingItem(undoItem.first,undoItem.second,false);
        redoActions.push(DeleteEmployee);
        //undoActions.pop();
    }
    else if(toPerformUndo == AddDepartment)
    {
        std::pair<DepartmentComponent*,int> undoItem = pb.getUndoTopItem(TODELETE);
        addExistingItem(undoItem.first,undoItem.second,false);
        redoActions.push(DeleteDepartment);
        //undoActions.pop();
    }
    else if(toPerformUndo == DeleteEmployee)
    {
        std::pair<DepartmentComponent*,int> undoItem = pb.getUndoTopItem(TOADD);
        for(auto i = employeeItems.begin();i!=employeeItems.end();i++)
        {
            if(i.value() == undoItem.first)
            {
                deleteItem(i.key(),false);
                break;
            }
        }
        redoActions.push(AddEmployee);
    }
    else if(toPerformUndo == DeleteDepartment)
    {
        std::pair<DepartmentComponent*,int> undoItem = pb.getUndoTopItem(TOADD);
        for(auto i = departmentItems.begin();i!=departmentItems.end();i++)
        {
            if(i.value() == undoItem.first)
            {
                deleteItem(i.key(),false);
                break;
            }
        }
        redoActions.push(AddDepartment);

    }

    //if(lastAction == AddEmployee)
    //{
    //    std::pair<DepartmentComponent*,int> undoItem = pb.getUndoTopItem(TODELETE);
    //    addExistingItem(undoItem.first,undoItem.second);
    //    redoActions.push(DeleteEmployee);
    //}
    //else if(lastAction == DeleteDepartment)
    //{
    //    std::pair<DepartmentComponent*,int> undoItem = pb.getUndoTopItem(TODELETE);
    //    addExistingItem(undoItem.first,undoItem.second);
    //    redoActions.push(DeleteDepartment);    }
    //else if(lastAction == DeleteEmployee)
    //{
    //    std::pair<DepartmentComponent*,int> undoItem = pb.getUndoTopItem(TOADD);
    //    for(auto i = employeeItems.begin();i!=employeeItems.end();i++)
    //    {
    //        if(i.value() == undoItem.first)
    //        {
    //            deleteItem(i.key());
    //            break;
    //        }
    //    }
    //    redoActions.push(AddEmployee);
    //}
    //else if(lastAction == AddDepartment)
    //{
    //    std::pair<DepartmentComponent*,int> undoItem = pb.getUndoTopItem(TOADD);
    //    for(auto i = departmentItems.begin();i!=departmentItems.end();i++)
    //    {
    //        if(i.value() == undoItem.first)
    //        {
    //            deleteItem(i.key());
    //            break;
    //        }
    //    }
    //}
}

void MainWindow::on_redoBtn_clicked()
{
    actions toPerformRedo = redoActions.top();
    redoActions.pop();
    if(toPerformRedo == DeleteEmployee)
    {
        std::pair<DepartmentComponent*,int> redoItem = pb.getRedoTopItem(TOADD);
        for(auto i = employeeItems.begin();i!=employeeItems.end();i++)
        {
            if(i.value() == redoItem.first)
            {
                deleteItem(i.key(),true);
                break;
            }
        }
    }
    else if(toPerformRedo == DeleteDepartment)
    {
        std::pair<DepartmentComponent*,int> redoItem = pb.getRedoTopItem(TOADD);

        for(auto i = departmentItems.begin();i!=departmentItems.end();i++)
        {
            if(i.value() == redoItem.first)
            {
                deleteItem(i.key(),true);
                break;
            }
        }
    }
    else if(toPerformRedo == AddEmployee)
    {
        std::pair<DepartmentComponent*,int> redoItem = pb.getRedoTopItem(TODELETE);

        addExistingItem(redoItem.first,redoItem.second,true);
    }
    else if(toPerformRedo == AddDepartment)
    {
        std::pair<DepartmentComponent*,int> redoItem = pb.getRedoTopItem(TODELETE);
        addExistingItem(redoItem.first,redoItem.second,true);
        //undoActions.push(DeleteDepartment);
    }
}
