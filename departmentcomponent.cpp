#include "departmentcomponent.h"


void Departments::add(DepartmentComponent *component)
{
      this->children.push_back(component);
      component->setParent(this);
      if (component->getParent() != nullptr)
      {
          numOfEmployees++;
      }
}

void Departments::remove(DepartmentComponent *component)
{
    children.erase(std::remove(children.begin(),children.end(),component));
    if (component->getParent() != nullptr)
    {
        numOfEmployees--;
    }
    component->setParent(nullptr);
    delete component;
}

void Departments::insert(DepartmentComponent *component, int index)
{
    if(this->numberOfLeaves() < index)
    {
        auto it = this->children.end()-1;
        this->children.insert(it,component);
    }
    else
    {
        auto it = this->children.begin() + index;
        this->children.insert(it, component);
    }
    component->setParent(this);
    if (component->getParent() != nullptr)
    {
        numOfEmployees++;
    }
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

int Departments::numberOfLeaves()
{
    return children.size();
}
