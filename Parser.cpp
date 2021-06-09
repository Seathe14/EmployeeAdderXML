#include "Parser.h"
#include "pugixml/pugixml.hpp"
#include <sstream>
parsedBase::parsedBase()
{
    departments = new Departments();
    current = 0;
}

void parsedBase::addRecord(DepartmentComponent *toAdd)
{
    if(toAdd->getParent() == nullptr) // if it's a new department
    {
        departments->add(toAdd);
        DepartmentComponent* copiedDepartments = new Departments(dynamic_cast<Departments&>(*departments));
        auto m = std::make_shared<Memento>(copiedDepartments);
        current++;
        changes.push_back(m);
    }
    else                             //if it's a new employee of an existing department
    {
        toAdd->getParent()->add(toAdd);
        DepartmentComponent* copiedDepartments = new Departments(dynamic_cast<Departments&>(*departments));
        auto m = std::make_shared<Memento>(copiedDepartments);
        current++;
        changes.push_back(m);
    }
}

std::shared_ptr<Memento> parsedBase::undo()
{
    if(current>0)
    {
        current--;
        auto m = changes[current];
        departments = new Departments(dynamic_cast<Departments&>(*m->departments));
        return m;
    }
    return {};
}
//
std::shared_ptr<Memento> parsedBase::redo()
{
    if(current + 1 < changes.size())
    {
        current++;
        auto m = changes[current];
        departments = new Departments(dynamic_cast<Departments&>(*m->departments));
        return m;
    }
    return {};
}


//void parsedBase::appendEmployeee(pugi::xml_node childNode, std::string surname, std::string name, std::string middleName, std::string functionInDep, std::string salary)
//{
//    childNode.append_child("surname").text().set(surname.c_str());
//    childNode.append_child("name").text().set(name.c_str());
//    childNode.append_child("middleName").text().set(middleName.c_str());
//    childNode.append_child("function").text().set(functionInDep.c_str());
//    childNode.append_child("salary").text().set(salary.c_str());
//}


void parsedBase::deleteRecord(DepartmentComponent *toDelete, int index, int action)
{
    if(toDelete->getParent()!=nullptr)
    {
        if(action == TODELETEDEPARTMENT)
        {
            departments->remove(toDelete);
        }
        else if(action == TODELETEEMPLOYEE)
        {
            toDelete->getParent()->remove(toDelete);
        }
    }
    DepartmentComponent* copiedDepartments = new Departments(dynamic_cast<Departments&>(*departments));
    auto m = std::make_shared<Memento>(copiedDepartments);
    changes.push_back(m);
    current++;
}


void parsedBase::loadFile(std::string fileName)
{
    fileName.append(".xml");
    auto result = doc.load_file(fileName.c_str());
    if(result.status!=pugi::status_ok)
        return;

    changes.clear();
    current = 0;
    departments->clear();
    pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
    decl.append_attribute("version") = "1.0";
    decl.append_attribute("encoding") = "UTF-8";
    std::ostringstream oss;
    oss.str("");
    oss << "//department";

    //Fill departments object
    for (pugi::xpath_node node : doc.select_nodes(oss.str().c_str()))
    {
        departments->add(new Departments(node.node().first_attribute().value()));
    }
    oss.str("");
    for (int i = 0; i < departments->numberOfLeaves(); i++)
    {
        oss << "//department[@name='" << dynamic_cast<Departments*>(departments->getComponent(i))->getName() << "']//employment";
        for (pugi::xpath_node node : doc.select_nodes(oss.str().c_str()))
        {
            std::string surname = node.node().child("surname").child_value();
            std::string name = node.node().child("name").child_value();
            std::string middleName = node.node().child("middleName").child_value();
            std::string functionInDep = node.node().child("function").child_value();
            std::string salary = node.node().child("salary").child_value();
            departments->getComponent(i)->add(new Employee(surname,name,middleName,functionInDep,atoi(salary.c_str())));
        }
        oss.str("");
    }
    DepartmentComponent* copiedDepartment = new Departments(dynamic_cast<Departments&>(*departments));
    auto m = std::make_shared<Memento>(copiedDepartment);
    changes.push_back(m);
}

void parsedBase::saveFile(std::string fileName)
{
    doc.remove_children();
    pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
    decl.append_attribute("version") = "1.0";
    decl.append_attribute("encoding") = "UTF-8";
    doc.append_child("departments");
    pugi::xml_node departmentsNode = doc.child("departments");
    for(int i =0;i<departments->numberOfLeaves();i++)
    {
         //pugi::xml_attribute attribute("name");
         departmentsNode.append_child("department").append_attribute("name").set_value(dynamic_cast<Departments*>(departments->getComponent(i))->getName().c_str());
         pugi::xml_node departmentNode = departmentsNode.find_child_by_attribute("department","name",(dynamic_cast<Departments*>(departments->getComponent(i))->getName().c_str()));
         pugi::xml_node employmentsNode = departmentNode.append_child("employments");
         for(int j = 0;j<departments->getComponent(i)->numberOfLeaves();j++)
         {
             pugi::xml_node employmentNode = employmentsNode.append_child("employment");
             employmentNode.append_child("surname").text().set(dynamic_cast<Employee*>(departments->getComponent(i)->getComponent(j))->getSurname().c_str());
             employmentNode.append_child("name").text().set(dynamic_cast<Employee*>(departments->getComponent(i)->getComponent(j))->getName().c_str());
             employmentNode.append_child("middleName").text().set(dynamic_cast<Employee*>(departments->getComponent(i)->getComponent(j))->getMiddleName().c_str());
             employmentNode.append_child("function").text().set(dynamic_cast<Employee*>(departments->getComponent(i)->getComponent(j))->getFunctionInDep().c_str());
             employmentNode.append_child("salary").text().set(std::to_string(dynamic_cast<Employee*>(departments->getComponent(i)->getComponent(j))->getSalary()).c_str());
         }
    }
    fileName = fileName + ".xml";
    doc.save_file(fileName.c_str(),"   ");
}

