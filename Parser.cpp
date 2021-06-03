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
            //std::string aa = ((Departments*)departmentss->getComponent(0))->getName();
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
                    //std::string nana = ((Employee*)departmentss->getComponent(i)->getComponent(i))->getName();
                    for (pugi::xml_node childNode : node.node())
                    {
                        std::string aa = childNode.next_sibling().child_value();
                        empl.addField(childNode.name(), childNode.child_value());
                        //std::cout << childNode.name() << std::endl;
                    }
                    departments[i].addEmployee(empl);
                }
                oss.str("");
            }
}

void parsedBase::addRecord(std::string departmentName, employee empl)
{
        std::string toSelect;
        //sprintf_s((char*)toSelect.c_str(), "//department[@name = \'%s\']/employments", departmentName.c_str());
        std::ostringstream oss;
        oss << "//department[@name = '" << departmentName << "']/employments";
        //sprintf_s((char*)toSelect.c_str(), 40 +departmentName.size(), "//department[@name = \'%s\']/employments",(char*)departmentName.c_str());
        pugi::xpath_node departmentwhereName = doc.select_node(oss.str().c_str());
        departmentwhereName.node().append_child("employment")
            .append_child("surname").text().set(empl.surname.c_str());
        pugi::xml_node childNode = departmentwhereName.node().last_child();

        //childNode.append_child("surname").text().set(surname.c_str());
        childNode.append_child("name").text().set(empl.name.c_str());
        childNode.append_child("middleName").text().set(empl.middleName.c_str());
        childNode.append_child("function").text().set(empl.functionInDep.c_str());
        childNode.append_child("salary").text().set(empl.salary.c_str());
        for(int i =0;i<departments.size();i++)
        {
            if (departments[i].getName() == departmentName)
            {
                departments[i].addEmployee(empl);
                break;
            }
        }
}

void parsedBase::deleteRecord(std::string departmentName, employee empl)
{
    std::ostringstream oss;
        //oss << "//department[@name = '" << departmentName << "']/employments";
        oss << "//department[@name='" << departmentName <<"']//employment[name = '" << empl.name << "'][surname='" << empl.surname << "'][middleName = '" << empl.middleName << "']"
        << "[function ='" << empl.functionInDep << "'][salary = '" << empl.salary << "']";
        pugi::xpath_node departmentwhereName = doc.select_node(oss.str().c_str());
        //departmentwhereName.node().remove_children();
        departmentwhereName.node().parent().remove_child(departmentwhereName.node());
        for(int i =0;i<departments.size();i++)
        {
            if (departments[i].getName() == departmentName)
            {
                departments[i].removeEmployee(empl);
                break;
            }
        }
}

void parsedBase::deleteDepartment(std::string departmentName)
{
    for(int i =0;i<departments.size();i++)
    {
        if(departments[i].getName() == departmentName)
        {
            for(int j = 0;j<departments[i].employees.size();j++)
            {
                deleteRecord(departmentName,departments[i].employees[j]);
            }
            departments.erase(std::remove(departments.begin(),departments.end(),departments[i]));
            break;
        }
    }
    std::ostringstream oss;
        oss << "//department[@name='" << departmentName <<"']";
        pugi::xpath_node departmentwhereName = doc.select_node(oss.str().c_str());
        //departmentwhereName.node().parent().remove_children();
        departmentwhereName.node().parent().remove_child(departmentwhereName.node());
}

void employee::addField(std::string fieldName, std::string fieldValue)
{
    if (fieldName == "surname")
                surname = fieldValue;
            if (fieldName == "name")
                name = fieldValue;
            if (fieldName == "middleName")
                middleName = fieldValue;
            if (fieldName == "function")
                functionInDep = fieldValue;
            if (fieldName == "salary")
            {
                salary = fieldValue;
                //salary = std::stoi(fieldValue);
            }
}
