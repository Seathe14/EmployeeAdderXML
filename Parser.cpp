#include "Parser.h"
#include "pugixml/pugixml.hpp"
#include <sstream>
parsedBase::parsedBase()
{
    departmentss = new Departments();
    doc.load_file("tst.xml");
    std::ostringstream oss;
            oss << "//department";
            for (pugi::xpath_node node : doc.select_nodes(oss.str().c_str()))
            {
                departmentss->add(new Departments(node.node().first_attribute().value()));
                department dep(node.node().first_attribute().value());
                departments.push_back(dep);
            }
            oss.str("");
            for (int i = 0; i < departments.size(); i++)
            {
                oss << "//department[@name='" << departments[i].getName() << "']//employment";
                for (pugi::xpath_node node : doc.select_nodes(oss.str().c_str()))
                {
                    employee empl;
                    std::string surname = node.node().child("surname").child_value();
                    std::string name = node.node().child("name").child_value();
                    std::string middleName = node.node().child("middleName").child_value();
                    std::string functionInDep = node.node().child("function").child_value();
                    std::string salary = node.node().child("salary").child_value();
                    departmentss->getComponent(i)->add(new Employee(surname,name,middleName,functionInDep,atoi(salary.c_str())));
                }
                oss.str("");
            }
}

void parsedBase::newAdd(DepartmentComponent *toAdd)
{
    if(toAdd->GetParent() == nullptr)
    {
        departmentss->add(toAdd);
        std::ostringstream oss;
        std::string departmentName = dynamic_cast<Departments*>(toAdd)->getName();
        oss << "//department";
        std::ostringstream attribute;
        attribute << "name";
        pugi::xpath_node departmentsNode = doc.select_node(oss.str().c_str());
        pugi::xml_node childNode = departmentsNode.node().append_child("department");
        childNode.append_attribute(attribute.str().c_str()).set_value(departmentName.c_str());
        childNode.append_child("employments");
        std::pair<DepartmentComponent*,int> pairToPush(toAdd,toAdd->GetParent()->numberOfLeaves()-1);
        undoStack.push(pairToPush);
        //.append_attribute(attribute.str().c_str()).set_value(departmentName.c_str());
    }
    else
    {
        std::ostringstream oss;
        std::string departmentName = dynamic_cast<Departments*>(toAdd->GetParent())->getName();
        std::string surname = dynamic_cast<Employee*>(toAdd)->getSurname();
        std::string name = dynamic_cast<Employee*>(toAdd)->getName();
        std::string middleName = dynamic_cast<Employee*>(toAdd)->getMiddleName();
        std::string functionInDep = dynamic_cast<Employee*>(toAdd)->getFunctionInDep();
        std::string salary = std::to_string(dynamic_cast<Employee*>(toAdd)->getSalary());
        oss << "//department[@name = '" << departmentName << "']/employments";
        pugi::xpath_node departmentwhereName = doc.select_node(oss.str().c_str());
        departmentwhereName.node().append_child("employment");
        pugi::xml_node childNode = departmentwhereName.node().last_child();
        appendEmployeee(childNode,surname,name,middleName,functionInDep,salary);
        std::pair<DepartmentComponent*,int> pairToPush(toAdd,toAdd->GetParent()->numberOfLeaves()-1);
        undoStack.push(pairToPush);
        //childNode.append_child("surname").text().set(surname.c_str());
        //childNode.append_child("name").text().set(name.c_str());
        //childNode.append_child("middleName").text().set(middleName.c_str());
        //childNode.append_child("function").text().set(functionInDep.c_str());
        //childNode.append_child("salary").text().set(salary.c_str());
    }
}

void parsedBase::insertRecord(DepartmentComponent *toAdd, int index)
{
    if(dynamic_cast<Departments*>(toAdd->GetParent())->getName() == "")
    {
        std::ostringstream oss;
        std::string departmentName = dynamic_cast<Departments*>(toAdd)->getName();
        oss << "//departments";
        std::ostringstream attribute;
        attribute << "name";
        pugi::xpath_node departmentsNode = doc.select_node(oss.str().c_str());
        int nodesSize = departmentsNode.node().select_nodes("department").size();
        int counter = 0;
        for(auto i : departmentsNode.node())
        {
            if(counter == index)
            {
                pugi::xml_node childNode = departmentsNode.node().insert_child_before("department",i);
                childNode.append_attribute(attribute.str().c_str()).set_value(departmentName.c_str());
                childNode.append_child("employments");
                for(int i =0;i<toAdd->numberOfLeaves();i++)
                {
                   insertRecord(toAdd->getComponent(i),i);
                }
                break;
            }
            counter++;
        }
        if(counter == nodesSize)
        {
            pugi::xml_node childNode = departmentsNode.node().append_child("department");
            childNode.append_attribute(attribute.str().c_str()).set_value(departmentName.c_str());
            childNode.append_child("employments");
            for(int i =0;i<toAdd->numberOfLeaves();i++)
            {
               insertRecord(toAdd->getComponent(i),i);
            }
        }
    }
    else
    {
        std::ostringstream oss;
        std::string departmentName = dynamic_cast<Departments*>(toAdd->GetParent())->getName();
        std::string surname = dynamic_cast<Employee*>(toAdd)->getSurname();
        std::string name = dynamic_cast<Employee*>(toAdd)->getName();
        std::string middleName = dynamic_cast<Employee*>(toAdd)->getMiddleName();
        std::string functionInDep = dynamic_cast<Employee*>(toAdd)->getFunctionInDep();
        std::string salary = std::to_string(dynamic_cast<Employee*>(toAdd)->getSalary());
        oss << "//department[@name = '" << departmentName << "']/employments";
        pugi::xpath_node departmentNode = doc.select_node(oss.str().c_str());


        int counter = 0;
        int nodesSize = departmentNode.node().select_nodes("employment").size();

        for(auto i : departmentNode.node())
        {
            if(counter == index)
            {
                pugi::xml_node childNode = departmentNode.node().insert_child_before("employment",i);
                appendEmployeee(childNode,surname,name,middleName,functionInDep,salary);
                //childNode.append_child("surname").text().set(surname.c_str());
                //childNode.append_child("name").text().set(name.c_str());
                //childNode.append_child("middleName").text().set(middleName.c_str());
                //childNode.append_child("function").text().set(functionInDep.c_str());
                //childNode.append_child("salary").text().set(salary.c_str());
                break;
            }
            counter++;
        }
        if(counter == nodesSize)
        {
            pugi::xml_node childNode = departmentNode.node().append_child("employment");
            appendEmployeee(childNode,surname,name,middleName,functionInDep,salary);
            //childNode.append_child("surname").text().set(surname.c_str());
            //childNode.append_child("name").text().set(name.c_str());
            //childNode.append_child("middleName").text().set(middleName.c_str());
            //childNode.append_child("function").text().set(functionInDep.c_str());
            //childNode.append_child("salary").text().set(salary.c_str());
        }
    }
}

void parsedBase::appendEmployeee(pugi::xml_node childNode, std::string surname, std::string name, std::string middleName, std::string functionInDep, std::string salary)
{
    childNode.append_child("surname").text().set(surname.c_str());
    childNode.append_child("name").text().set(name.c_str());
    childNode.append_child("middleName").text().set(middleName.c_str());
    childNode.append_child("function").text().set(functionInDep.c_str());
    childNode.append_child("salary").text().set(salary.c_str());
}


void parsedBase::newDelete(DepartmentComponent *toDelete, int index)
{
    if(toDelete->GetParent()!=nullptr)
    {
        if(dynamic_cast<Departments*>(toDelete->GetParent())->getName() == "")
        {
            std::string departmentName = dynamic_cast<Departments*>(toDelete)->getName();
            std::ostringstream oss;
            oss << "//department[@name='" << departmentName <<"']";
            pugi::xpath_node departmentwhereName = doc.select_node(oss.str().c_str());
            //departmentwhereName.node().parent().remove_children();
            DepartmentComponent* deletedEmployee = toDelete->makeClone();
            departmentwhereName.node().parent().remove_child(departmentwhereName.node());
            std::pair<DepartmentComponent*,int> pairToPush(deletedEmployee,index);
            undoStack.push(pairToPush);
            departmentss->remove(toDelete);
        }
        else
        {
            std::ostringstream oss;
            std::string departmentName = dynamic_cast<Departments*>(toDelete->GetParent())->getName();
            std::string surname = dynamic_cast<Employee*>(toDelete)->getSurname();
            std::string name = dynamic_cast<Employee*>(toDelete)->getName();
            std::string middleName = dynamic_cast<Employee*>(toDelete)->getMiddleName();
            std::string functionInDep = dynamic_cast<Employee*>(toDelete)->getFunctionInDep();
            std::string salary = std::to_string(dynamic_cast<Employee*>(toDelete)->getSalary());
            oss << "//department[@name='" << departmentName <<"']//employment[name = '" << name << "'][surname='" << surname << "'][middleName = '" << middleName << "']"
            << "[function ='" << functionInDep << "'][salary = '" << salary << "']";
            pugi::xpath_node departmentwhereName = doc.select_node(oss.str().c_str());
            departmentwhereName.node().parent().remove_child(departmentwhereName.node());
            DepartmentComponent* deletedEmployee = toDelete->makeClone();
            std::pair<DepartmentComponent*,int> pairToPush(deletedEmployee,index);
            //std::unique_ptr<std::pair<DepartmentComponent*,int>> pairToPush = std::make_unique<std::pair<DepartmentComponent*,int>>(toDelete,index);
            //std::make_unique()
            undoStack.push(pairToPush);
            toDelete->GetParent()->remove(toDelete);
        }
    }
}

std::pair<DepartmentComponent *, int> parsedBase::getUndoTopItem(int action)
{
    std::pair<DepartmentComponent *,int> newPair = undoStack.top();
    redoStack.push(undoStack.top());
    //if(action == TOADD)
    //    redoStack.top().first = undoStack.top().first->makeClone();
    undoStack.pop();
    return newPair;
}

std::pair<DepartmentComponent *, int> parsedBase::getRedoTopItem()
{
    std::pair<DepartmentComponent *,int> newPair = redoStack.top();
    //undoStack.pop();
    //undoStack.push(redoStack.top());
    redoStack.pop();
    return newPair;

}

