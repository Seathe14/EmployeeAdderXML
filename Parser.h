#ifndef PARSER_H
#define PARSER_H

#endif // PARSER_H
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include "pugixml/pugixml.hpp"
#include "departmentcomponent.h"
#define TODELETE 1
#define TOADD 2

class parsedBase
{
public:
    parsedBase();
    void newAdd(DepartmentComponent* toAdd);
    void insertRecord(DepartmentComponent* toAdd,int index);
    bool isUndoEmpty() { return undoStack.empty();}
    bool isRedoEmpty() { return redoStack.empty();}
    void appendEmployeee(pugi::xml_node, std::string surname,std::string name,std::string middleName,std::string functionInDep,std::string salary);
    void newDelete(DepartmentComponent* toDelete, int index);
    std::pair<DepartmentComponent*,int> getUndoTopItem(int action);
    std::pair<DepartmentComponent*,int> peekRedoItem();
    std::pair<DepartmentComponent*,int> getRedoTopItem(int action);
    void setToPush(bool toPush) {this->toPush = toPush;}
    void saveChanges(std::string fileName)
    {
        doc.save_file(fileName.c_str(),"   ");
    }
    ~parsedBase()
    {
        //pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
        //decl.append_attribute("version") = "1.0";
        //decl.append_attribute("encoding") = "UTF-8";
        //doc.save_file("save_file_output.xml","   ");
    };
    DepartmentComponent* departments;
private:
    std::stack<std::pair<DepartmentComponent*,int>> undoStack;
    std::stack<std::pair<DepartmentComponent*,int>> redoStack;
    bool toPush;

    pugi::xml_document doc;
};
