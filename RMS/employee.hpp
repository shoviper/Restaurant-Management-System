#ifndef EMPLOYEE_HPP
#define EMPLOYEE_HPP

#include <string>

class Employee
{
protected:
    std::string name;
    std::string surname;
    int id;
    int hours = 8;
    double wages;

public:
    Employee() = delete;
    Employee(std::string n, std::string sn, int id, double w = 0.0) : name(n), surname(sn), id(id), wages(w) {}

    virtual ~Employee() {}

    std::string getName();
    std::string getSur();
    int getID();
    double getWages();
    int getHours();

    std::string getEmployee();

    void setWages(double w);
    virtual void setHours(int hr = 0) {}

    double getSalary() const;
};

std::string Employee::getName()
{
    return name;
}
std::string Employee::getSur()
{
    return surname;
}
int Employee::getID()
{
    return id;
}
double Employee::getWages()
{
    return wages;
}
int Employee::getHours()
{
    return hours;
}

std::string Employee::getEmployee()
{
    return name + " " + surname;
}

void Employee::setWages(double w)
{
    wages = w;
}
double Employee::getSalary() const
{
    return hours * wages * 30;
}

class Part_time : public Employee
{
public:
    Part_time() = delete;
    Part_time(std::string n, std::string sn, int id, double w, int hr) : Employee(n, sn, id, w) { setHours(hr); }

    ~Part_time() {}

    void setHours(int hr) override;
};

void Part_time::setHours(int hr)
{
    hours = hr;
}

#endif // EMPLOYEE_HPP