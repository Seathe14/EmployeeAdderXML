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
    virtual void add(DepartmentComponent* component) = 0;
    virtual void remove(DepartmentComponent* component) = 0;
    virtual void clear() = 0;
    virtual DepartmentComponent* getComponent(int componentIndex) { return nullptr; }
    virtual ~DepartmentComponent() {}
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
    void add(DepartmentComponent* component) override {};
    void remove(DepartmentComponent* component) override {};
    void clear() override {};
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
    Departments(const Departments& toCopy);
    ~Departments()
    {
        for (int i =0;i<children.size();i++)
        {
            delete children[i];
        }
    }
    std::string getName() { return name; }
    float getAvgSalary() { return avgSalary; }
    int getNumOfEmployees() {return numOfEmployees;}
private:
    std::string name;
    float avgSalary;
    int numOfEmployees;

public:
    void add(DepartmentComponent* component) override;
    void remove(DepartmentComponent* component) override;
    void clear()override;
    DepartmentComponent* getComponent(int componentIndex) override
    {
        return (DepartmentComponent*)children[componentIndex];
    }
    float countAvgSalary();
    int numberOfLeaves();

};
#endif // DEPARTMENTCOMPONENT_H
