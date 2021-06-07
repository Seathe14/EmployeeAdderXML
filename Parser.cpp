#include "Parser.h"
#include "pugixml/pugixml.hpp"
#include <sstream>
parsedBase::parsedBase()
{
    departments = new Departments();
    doc.load_file("tst.xml");
    pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
    decl.append_attribute("version") = "1.0";
    decl.append_attribute("encoding") = "UTF-8";
    std::ostringstream oss;
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
}

void parsedBase::addRecord(DepartmentComponent *toAdd)
{
    if(toAdd->getParent() == nullptr) // if it's a new department
    {
        departments->add(toAdd);
        std::ostringstream oss;
        std::string departmentName = dynamic_cast<Departments*>(toAdd)->getName();
        oss << "//departments";
        std::ostringstream attribute;
        attribute << "name";
        pugi::xpath_node departmentsNode = doc.select_node(oss.str().c_str());
        pugi::xml_node childNode = departmentsNode.node().append_child("department");
        childNode.append_attribute(attribute.str().c_str()).set_value(departmentName.c_str());
        childNode.append_child("employments");
        std::pair<DepartmentComponent*,int> pairToPush(toAdd,toAdd->getParent()->numberOfLeaves()-1);
        undoStack.push_back(pairToPush);
    }
    else                             //if it's a new employee of an existing department
    {
        std::ostringstream oss;
        std::string departmentName = dynamic_cast<Departments*>(toAdd->getParent())->getName();
        std::string surname = dynamic_cast<Employee*>(toAdd)->getSurname();
        std::string name = dynamic_cast<Employee*>(toAdd)->getName();
        std::string middleName = dynamic_cast<Employee*>(toAdd)->getMiddleName();
        std::string functionInDep = dynamic_cast<Employee*>(toAdd)->getFunctionInDep();
        std::string salary = std::to_string(dynamic_cast<Employee*>(toAdd)->getSalary());
        oss << "//department[@name = '" << departmentName << "']/employments";
        pugi::xpath_node employmentsNode = doc.select_node(oss.str().c_str());
        employmentsNode.node().append_child("employment");
        pugi::xml_node childNode = employmentsNode.node().last_child();
        appendEmployeee(childNode,surname,name,middleName,functionInDep,salary);
        std::pair<DepartmentComponent*,int> pairToPush(toAdd,toAdd->getParent()->numberOfLeaves()-1);
        undoStack.push_back(pairToPush);

    }
}

void parsedBase::insertRecord(DepartmentComponent *toAdd, int index)
{
    if(dynamic_cast<Departments*>(toAdd->getParent())->getName() == "") // if we're inserting a department which has a reference to parent
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
        if(toPush)
        {
            std::pair<DepartmentComponent*,int> pairToPush(toAdd,toAdd->getParent()->numberOfLeaves()-1);
            undoStack.push_back(pairToPush);
        }
    }
    else                //  if we're inserting an employee.
    {
        std::ostringstream oss;
        std::string departmentName = dynamic_cast<Departments*>(toAdd->getParent())->getName();
        std::string surname = dynamic_cast<Employee*>(toAdd)->getSurname();
        std::string name = dynamic_cast<Employee*>(toAdd)->getName();
        std::string middleName = dynamic_cast<Employee*>(toAdd)->getMiddleName();
        std::string functionInDep = dynamic_cast<Employee*>(toAdd)->getFunctionInDep();
        std::string salary = std::to_string(dynamic_cast<Employee*>(toAdd)->getSalary());
        oss << "//department[@name = '" << departmentName << "']/employments";
        pugi::xpath_node employmentsNode = doc.select_node(oss.str().c_str());


        int counter = 0;
        int nodesSize = employmentsNode.node().select_nodes("employment").size();

        for(auto i : employmentsNode.node())
        {
            if(counter == index)
            {
                pugi::xml_node childNode = employmentsNode.node().insert_child_before("employment",i);
                appendEmployeee(childNode,surname,name,middleName,functionInDep,salary);
                break;
            }
            counter++;
        }
        if(counter == nodesSize)
        {
            pugi::xml_node childNode = employmentsNode.node().append_child("employment");
            appendEmployeee(childNode,surname,name,middleName,functionInDep,salary);
        }
        if(toPush)
        {
            std::pair<DepartmentComponent*,int> pairToPush(toAdd,toAdd->getParent()->numberOfLeaves()-1);
            undoStack.push_back(pairToPush);
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


void parsedBase::deleteRecord(DepartmentComponent *toDelete, int index)
{
    if(toDelete->getParent()!=nullptr)
    {
        if(dynamic_cast<Departments*>(toDelete->getParent())->getName() == "")
        {
            std::string departmentName = dynamic_cast<Departments*>(toDelete)->getName();
            std::ostringstream oss;
            oss << "//department[@name='" << departmentName <<"']";
            pugi::xpath_node departmentNode = doc.select_node(oss.str().c_str());
            DepartmentComponent* copyOfDeletedDepartment = toDelete->makeClone();
            auto copiedUndoStack = undoStack;
            auto copiedRedoStack = redoStack;
            int redoStackSize = copiedRedoStack.size();
            int undoStackSize = copiedUndoStack.size();
            for(int i =0;i<redoStackSize;i++) // the same fix as in getRedoTopItem otherwise it doesn't work
            {
                if(copiedRedoStack.empty())
                    break;
                auto copiedPair = copiedRedoStack.back();
                copiedRedoStack.pop_back();
                if(dynamic_cast<Departments*>(copiedPair.first->getParent())->getName() == dynamic_cast<Departments*>(toDelete)->getName())
                {
                    if(redoStack[i].first == toDelete)
                    {
                        redoStack[i].first = copyOfDeletedDepartment;
                    }
                    if(!toDelete->contains(copiedPair.first))
                    {
                        copiedPair.first->setParent(copyOfDeletedDepartment);
                        copiedPair.first->getParent()->add(copiedPair.first);
                    }
                    else
                    {
                        int foundIndex = toDelete->find(copiedPair.first);
                        DepartmentComponent* employee = copiedPair.first->getParent()->getComponent(foundIndex)->makeClone();
                        redoStack[redoStackSize- i - 1].first = employee;
                        redoStack[redoStackSize-1-i].first->setParent(copyOfDeletedDepartment);
                    }
                }
                else if(dynamic_cast<Departments*>(copiedPair.first->getParent())->getName() == "")
                    if(dynamic_cast<Departments*>(copiedPair.first)->getName() == dynamic_cast<Departments*>(toDelete)->getName())
                    {
                        DepartmentComponent* dep = copiedPair.first->getParent()->getComponent(index);
                        dep = copyOfDeletedDepartment;
                    }
            }
            for(int i = 0;i<undoStackSize;i++)
            {
                if(copiedUndoStack.empty())
                    break;
                std::pair<DepartmentComponent*,int> copiedPair = copiedUndoStack.back();
                copiedUndoStack.pop_back();
                if(copiedPair.first->getParent() == toDelete)
                {
                    copiedPair.first->setParent(copyOfDeletedDepartment);
                }
            }
            departmentNode.node().parent().remove_child(departmentNode.node());
            std::pair<DepartmentComponent*,int> pairToPush(copyOfDeletedDepartment,index);
            if(toPush)
                undoStack.push_back(pairToPush);
            departments->remove(toDelete);
        }
        else
        {
            std::ostringstream oss;
            std::string departmentName = dynamic_cast<Departments*>(toDelete->getParent())->getName();
            std::string surname = dynamic_cast<Employee*>(toDelete)->getSurname();
            std::string name = dynamic_cast<Employee*>(toDelete)->getName();
            std::string middleName = dynamic_cast<Employee*>(toDelete)->getMiddleName();
            std::string functionInDep = dynamic_cast<Employee*>(toDelete)->getFunctionInDep();
            std::string salary = std::to_string(dynamic_cast<Employee*>(toDelete)->getSalary());
            oss << "//department[@name='" << departmentName <<"']//employment[name = '" << name << "'][surname='" << surname << "'][middleName = '" << middleName << "']"
            << "[function ='" << functionInDep << "'][salary = '" << salary << "']";
            pugi::xpath_node employeeNode = doc.select_node(oss.str().c_str());
            employeeNode.node().parent().remove_child(employeeNode.node());
            DepartmentComponent* copyOfDeletedEmployee = toDelete->makeClone();
            std::pair<DepartmentComponent*,int> pairToPush(copyOfDeletedEmployee,index);
            if(toPush)
                undoStack.push_back(pairToPush);
            toDelete->getParent()->remove(toDelete);
        }
    }
}

std::pair<DepartmentComponent *, int> parsedBase::getUndoTopItem(int action)
{
    toPush = false;
    std::pair<DepartmentComponent *,int> newPair = undoStack.back();
    redoStack.push_back(undoStack.back());
    if(action == TOADD)
    {
        redoStack.back().first = undoStack.back().first->makeClone();
    }
    undoStack.pop_back();
    return newPair;
}


std::pair<DepartmentComponent *, int> parsedBase::getRedoTopItem(int action)
{
    toPush = true;
    std::pair<DepartmentComponent *,int> newPair = redoStack.back();
    auto clonedStack = redoStack;
    if(dynamic_cast<Departments*>(newPair.first->getParent())->getName() == "") // To fix add deparment -> add employee -> undo -> undo -> redo -> redo
    {                                                                           // Basically fixing when employee is deleted before department and it loses its parent pointer.
        int clonedStackSize = clonedStack.size();
        for(int i =0;i<clonedStackSize;i++)
        {
            auto clonedPair = clonedStack.back();
            clonedStack.pop_back();
            if(dynamic_cast<Departments*>(clonedPair.first->getParent())->getName() == dynamic_cast<Departments*>(newPair.first)->getName()
                    && clonedPair.first->getParent()!= newPair.first)
            {
                clonedPair.first->setParent(newPair.first);
            }
        }
    }
    redoStack.pop_back();
    return newPair;
}

