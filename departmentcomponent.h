#ifndef DEPARTMENTCOMPONENT_H
#define DEPARTMENTCOMPONENT_H
#include <iostream>
#include <vector>
#include <algorithm>
class DepartmentComponent {
protected:
    DepartmentComponent* parent;
public:
    DepartmentComponent():parent(nullptr){}
    void setParent(DepartmentComponent* parent)
    {
        this->parent = parent;
    }
    DepartmentComponent* getParent()const
    {
        return this->parent;
    }
    virtual bool contains(DepartmentComponent* component){ return 0;}
    virtual void add(DepartmentComponent* component) {}
    virtual void remove(DepartmentComponent* component) {}
    virtual void insert(DepartmentComponent* component,int index) {}
    virtual int find(DepartmentComponent* component) {return -1;}
    virtual void clear(){}
    virtual DepartmentComponent* getComponent(int componentIndex) { return nullptr; }
    virtual void displayEmployeeInfo() {}
    virtual int numberOfLeaves() {return 0;}
    virtual float countAvgSalary() { return 0; }
    virtual DepartmentComponent* makeClone() = 0;
    virtual void setComponent(DepartmentComponent*,int index) {}
    virtual ~DepartmentComponent() {};
};

class Employee : public DepartmentComponent
{
public:
    Employee(std::string surname, std::string name, std::string middleName, std::string functionInDep, int salary) : surname(surname),
        name(name), middleName(middleName), functionInDep(functionInDep), salary(salary) {}
    std::string getSurname() { return surname; }
    std::string getName() { return name; }
    std::string getMiddleName() { return middleName; }
    std::string getFunctionInDep() { return functionInDep; }
    std::string FIO(){return surname + " " + name + " " + middleName;}
    int getSalary() { return salary; }
    DepartmentComponent* makeClone() override
    {
        return new Employee(*this);
    }
private:
    std::string surname;
    std::string name;
    std::string middleName;
    std::string functionInDep;
    int salary;
};
class Departments : public DepartmentComponent
{
protected:
    std::vector<DepartmentComponent*> children;
public:
    Departments() = default;
    Departments(std::string name) : name(name),numOfEmployees(0) {}
    ~Departments()
    {
        for (int i =0;i<children.size();i++)
        {
            delete children[i];
        }
    }
    std::string getName() { return name; }
    float getAvgSalary() { return avgSalary; }
    DepartmentComponent* makeClone() override
    {
        if(this->name != "")
        {
            Departments* newDep = new Departments(this->name);
            newDep->setParent(this->parent);
            for (int i = 0; i < this->children.size(); i++)
            {
                DepartmentComponent* employeeToClone = this->children[i]->makeClone();
                newDep->add(employeeToClone);
            }
            return newDep;
        }
        else
        {
            Departments* newDep = new Departments();
            for(int i =0;i<this->children.size();i++)
            {
                newDep->children.push_back(this->children[i]->makeClone());
            }
            return newDep;
        }
    }
    int getNumOfEmployees() {return numOfEmployees;}
private:
    std::string name;
    float avgSalary;
    int numOfEmployees;

public:
    void add(DepartmentComponent* component) override;
    void remove(DepartmentComponent* component) override;
    void insert(DepartmentComponent* component, int index) override;
    bool contains(DepartmentComponent* component) override;
    int find(DepartmentComponent* component) override;
    void setComponent(DepartmentComponent* component,int index)override;
    void clear()override;
    DepartmentComponent* getComponent(int componentIndex) override
    {
        return (DepartmentComponent*)children[componentIndex];
    }
    void displayEmployeeInfo() override;
    float countAvgSalary()override;
    int numberOfLeaves() override;

};
#endif // DEPARTMENTCOMPONENT_H
