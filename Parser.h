#ifndef PARSER_H
#define PARSER_H

#endif // PARSER_H
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include "pugixml/pugixml.hpp"
#include "departmentcomponent.h"
#define TODELETEEMPLOYEE 1
#define TOADDEMPLOYEE 2
#define TOADDDEPARTMENT 3
#define TODELETEDEPARTMENT 4
class parsedBase
{
public:
    parsedBase();
    void addRecord(DepartmentComponent* toAdd);
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
    ~parsedBase()
    {

    };
    DepartmentComponent* departments;
private:
    std::vector<std::pair<DepartmentComponent*,int>> undoStack;
    std::vector<std::pair<DepartmentComponent*,int>> redoStack;
    bool toPush;

    pugi::xml_document doc;
};
