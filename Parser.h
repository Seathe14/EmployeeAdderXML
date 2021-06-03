#ifndef PARSER_H
#define PARSER_H

#endif // PARSER_H
#include <vector>
#include <string>
#include <algorithm>
#include "pugixml/pugixml.hpp"
#include "departmentcomponent.h"
class employee
{
public:
    employee() = default;
    employee(std::string surname,
            std::string name,
            std::string middleName,
            std::string functionInDep,
            std::string salary)
            :
              surname(surname),name(name), middleName(middleName),
              functionInDep(functionInDep), salary(salary){}
    void addField(std::string fieldName, std::string fieldValue);
    std::string getSurname() {return surname;}
    std::string getName() {return name;}
    std::string getMiddleName() {return middleName;}
    std::string getFunction() {return functionInDep;}
    std::string getSalary() {return salary;}
    bool operator==(const employee& rightEmpl) const
    {
        if(this->surname == rightEmpl.surname && this->name == rightEmpl.name && this->middleName == rightEmpl.middleName
                && this->functionInDep == rightEmpl.functionInDep && this->salary == rightEmpl.salary)
            return 1;
        return 0;
    }

public:
    std::string surname;
    std::string name;
    std::string middleName;
    std::string functionInDep;
    std::string salary;
};

class department
{
public:
    department(std::string name)
    {
        this->name = name;
    }
    void addEmployee(employee empl)
    {
        employees.push_back(empl);
    }
    void removeEmployee(employee empl)
    {
       auto element = std::find(employees.begin(),employees.end(),empl);
       employees.erase(element);
    }
    std::string getName() { return name; }
    int getEmployeeNum() {return employees.size();}
    std::vector<employee>employees;
    bool operator==(const department& second)
    {
        return this->name == second.name;
    }
private:
    std::string name;
};


class parsedBase
{
public:
    parsedBase();
    std::vector<department>departments;
    void addRecord(std::string departmentName, employee empl);
    void deleteRecord(std::string departmentName,employee empl);
    void deleteDepartment(std::string departmentName);
    void saveChanges()
    {
        pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
        decl.append_attribute("version") = "1.0";
        decl.append_attribute("encoding") = "UTF-8";
        doc.save_file("save_file_output.xml","   ");
    }
    ~parsedBase()
    {
        pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
        decl.append_attribute("version") = "1.0";
        decl.append_attribute("encoding") = "UTF-8";
        doc.save_file("save_file_output.xml","   ");
    };
    DepartmentComponent* departmentss;
private:
    pugi::xml_document doc;
};