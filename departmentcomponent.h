#ifndef DEPARTMENTCOMPONENT_H
#define DEPARTMENTCOMPONENT_H
#include <iostream>
#include <vector>
class DepartmentComponent {
protected:
    DepartmentComponent* parent;
public:
    void setParent(DepartmentComponent* parent)
    {
        this->parent = parent;
    }
    DepartmentComponent* GetParent()const
    {
        return this->parent;
    }
    virtual void add(DepartmentComponent* component) {}
    virtual void remove(DepartmentComponent* component) {}
    virtual DepartmentComponent* getComponent(int componentIndex) { return nullptr; }
    virtual void displayEmployeeInfo() {}
    virtual float countAvgSalary() { return 0; }
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
    int getSalary() { return salary; }
    //void displayEmployeeInfo()
    //{
    //    std::cout << "FIO: " << surname << " " << name << " " << middleName << " , function " << functionInDep << ",salary " << salary << std::endl;
    //}
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
    Departments(std::string name) : name(name) {}
    std::string getName() { return name; }
    int getAvgSalary() { return avgSalary; }
private:
    std::string name;
    float avgSalary;
public:
    void add(DepartmentComponent* component) override;
    void remove(DepartmentComponent* component) override;
    DepartmentComponent* getComponent(int componentIndex) override
    {
        return (DepartmentComponent*)children[componentIndex];
    }
    void displayEmployeeInfo() override;
    float countAvgSalary()override;

};
#endif // DEPARTMENTCOMPONENT_H
