#include "departmentcomponent.h"


void Departments::add(DepartmentComponent *component)
{
    this->children.push_back(component);
    component->setParent(this);
}

void Departments::remove(DepartmentComponent *component)
{
    children.erase(std::remove(children.begin(),children.end(),component));
    component->setParent(nullptr);
}

void Departments::displayEmployeeInfo()
{
    for (auto i : children)
    {
        DepartmentComponent* employeeInfo = i;
        employeeInfo->displayEmployeeInfo();
    }
}

float Departments::countAvgSalary()
{

    avgSalary = 0;
    for (int i = 0; i < children.size(); i++)
    {
        avgSalary += ((Employee*)children[i])->getSalary();
    }
    return avgSalary /= children.size();
}
