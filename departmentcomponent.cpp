#include "departmentcomponent.h"


Departments::Departments(const Departments &toCopy)
{
    if(toCopy.getParent() == nullptr)
    {
        for(int i =0;i<toCopy.children.size();i++)
        {
            std::string name = dynamic_cast<Departments*>(toCopy.children[i])->getName();
            DepartmentComponent* newDep = new Departments(name);
            this->add(newDep);
            for(int j =0;j<toCopy.children[i]->numberOfLeaves();j++)
            {
                DepartmentComponent* toPush = new Employee (dynamic_cast<Employee&>(*toCopy.children[i]->getComponent(j)));
                newDep->add(toPush);
            }

        }
    }
}

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

void Departments::clear()
{
    int size = numberOfLeaves();
    for (int i = 0; i < size; i++)
    {
        this->remove(children[0]);
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
