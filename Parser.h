#ifndef PARSER_H
#define PARSER_H

#endif // PARSER_H
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include <memory>
#include "pugixml/pugixml.hpp"
#include "departmentcomponent.h"
#define TODELETEEMPLOYEE 1
#define TOADDEMPLOYEE 2
#define TOADDDEPARTMENT 3
#define TODELETEDEPARTMENT 4
class Memento
{
private:
    DepartmentComponent* departments;
    //pugi::xml_node node;
public:
    Memento(DepartmentComponent* depComponent,pugi::xml_node nodeToCopy)
    {
        departments = depComponent;
        //node = nodeToCopy;
    }
    friend class parsedBase;
};
class parsedBase
{
public:
    parsedBase();
    std::shared_ptr<Memento> addRecord(DepartmentComponent* toAdd);
    std::shared_ptr<Memento> undo();
    std::shared_ptr<Memento> redo();
    void insertRecord(DepartmentComponent* toAdd,int index);
    bool isUndoEmpty() { return undoStack.empty();}
    bool isRedoEmpty() { return redoStack.empty();}
    void appendEmployeee(pugi::xml_node, std::string surname,std::string name,std::string middleName,std::string functionInDep,std::string salary);
    void deleteRecord(DepartmentComponent* toDelete, int index,int component);
    std::pair<DepartmentComponent*,int> getUndoTopItem(int action);
    std::pair<DepartmentComponent*,int> peekRedoItem();
    std::pair<DepartmentComponent*,int> getRedoTopItem(int action);
    void setToPush(bool toPush) {this->toPush = toPush;}
    void loadFile(std::string fileName);
    void saveFile(std::string fileName);
    void fileSave(std::string fileName);
    //void recordDepartments
    ~parsedBase()
    {

    };
    DepartmentComponent* departments;
private:
    std::vector<std::pair<DepartmentComponent*,int>> undoStack;
    std::vector<std::pair<DepartmentComponent*,int>> redoStack;
    bool toPush;
    std::vector<std::shared_ptr<Memento>> changes;
    int current;
    pugi::xml_document doc;
};
