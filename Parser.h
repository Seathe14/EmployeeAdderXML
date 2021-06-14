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
#define TODELETEDEPARTMENT 2
class Memento
{
private:
    DepartmentComponent* departments;
public:
    Memento(DepartmentComponent *depComponent)
    {
        departments = depComponent;
    }
    ~Memento() {delete departments;}
    friend class parsedBase;
};
class parsedBase
{
public:
    parsedBase();
    void addRecord(DepartmentComponent* toAdd);
    std::shared_ptr<Memento> undo();
    std::shared_ptr<Memento> redo();
    void appendEmployeee(pugi::xml_node, std::string surname,std::string name,std::string middleName,std::string functionInDep,std::string salary);
    void deleteRecord(DepartmentComponent* toDelete, int index,int component);
    void loadFile(std::string fileName);
    void saveFile(std::string fileName);
    DepartmentComponent* departments;
    ~parsedBase();
private:
    std::vector<std::shared_ptr<Memento>> changes;
    int current;
    pugi::xml_document doc;
};
