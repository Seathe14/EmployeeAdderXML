#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QMap>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QTimer>
#include <QFileDialog>
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


private:
    void deleteItem(QTreeWidgetItem *itm);
    void addEmployeeItem();
    void fillItems();
    void clearItems();
private slots:

    void on_addEmplBtn_clicked();

    void on_treeWidget_itemActivated(QTreeWidgetItem *item, int column);

    void on_delEmplBtn_clicked();

    void on_addDepBtn_clicked();

    void on_undoBtn_clicked();

    void on_redoBtn_clicked();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_saveBtn_clicked();

    void on_loadBtn_clicked();

private:
    Ui::MainWindow *ui;
    QMap<QTreeWidgetItem*, DepartmentComponent*> departmentItems;
    QMap<QTreeWidgetItem*, DepartmentComponent*> employeeItems;
    parsedBase pb;
};
#endif // MAINWINDOW_H
