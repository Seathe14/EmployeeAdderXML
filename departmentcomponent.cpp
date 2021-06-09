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

bool Departments::contains(DepartmentComponent *component)
{
    for(int i =0;i<this->numOfEmployees;i++)
    {
        if(this->getComponent(i) == component)
        {
            return true;
        }
    }
    return false;
}

int Departments::find(DepartmentComponent *component)
{
    for(int i =0;i<this->numOfEmployees;i++)
    {
        if(this->getComponent(i) == component)
        {
            return i;
        }
    }
    return -1;
}

void Departments::setComponent(DepartmentComponent *component, int index)
{
    children[index] = component;
}

void Departments::clear()
{
    int size = numberOfLeaves();
    for (int i = 0; i < size; i++)
    {
        this->remove(children[0]);
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
    if(children.size() == 0)
    {
        return 0;
    }
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
