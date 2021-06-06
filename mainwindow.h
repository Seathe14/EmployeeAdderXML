#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QMap>
#include <QTreeWidgetItem>
#include "Parser.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public:
    void deleteItem(QTreeWidgetItem *item);
    void addExistingItem(DepartmentComponent* itm,int index);
private slots:
    void on_addEmplBtn_clicked();

    void on_treeWidget_itemActivated(QTreeWidgetItem *item, int column);

    void on_delEmplBtn_clicked();

    void on_addDepBtn_clicked();

    void on_undoBtn_clicked();

    void on_redoBtn_clicked();


private:
    Ui::MainWindow *ui;
    enum actions{DeleteEmployee,DeleteDepartment,AddEmployee,AddDepartment} lastAction;
    actions redoAction;
    QMap<QTreeWidgetItem*, QString> departments;
    QMap<QTreeWidgetItem*, DepartmentComponent*> departmentItems;
    QMap<QTreeWidgetItem*, DepartmentComponent*> employeeItems;
    std::stack<actions>undoActions;
    std::stack<actions>redoActions;

    QMap<QTreeWidgetItem*,employee> employeeChildren;
    void fillItems();
    void addRecord(std::string departmentName, employee empl);
    parsedBase pb;
};
#endif // MAINWINDOW_H
