#ifndef ORDER_HPP
#define ORDER_HPP

#include <fstream>
#include <map>
#include <sstream>

#include "menu.hpp"
#include "configure.hpp"

class Order
{
private:
    int orderNo;
    bool takeAway = false;
    double total, income;
    std::map<Food, int> orderList;

public:
    explicit Order(const int no) : orderNo(no) {}
    ~Order() {}

    void setOrderList(std::map<Food, int> l);
    void setTotal(double t);
    void setIncome(double i);

    int getOrderNo();
    bool getTakeAway();
    std::map<Food, int> getOrderList();
    double getIncome();
    double getTotal();
    double calculateIncome();
    double calculateTotal();

    void addToList(const Food &menu, int quantity);
    void removeInList(const Food &menu);
    void makeTakeAway();
    void getDetail();
};

void Order::setOrderList(std::map<Food, int> l)
{
    orderList = l;
}

void Order::setTotal(double t)
{
    total = t;
}

void Order::setIncome(double i)
{
    income = i;
}

int Order::getOrderNo()
{
    return orderNo;
}

bool Order::getTakeAway()
{
    return takeAway;
}

std::map<Food, int> Order::getOrderList()
{
    return orderList;
}

double Order::getIncome()
{
    return income;
}

double Order::getTotal()
{
    return total;
}

double Order::calculateIncome()
{
    income = 0.0;

    for (auto const &[key, val] : orderList)
    {
        income += key.price * val;
    }
    income *= config.service_charge;
    return income;
}

double Order::calculateTotal()
{
    total = calculateIncome() * config.tax_rate;
    return total;
}

void Order::addToList(const Food &menu, int quantity)
{
    orderList.insert_or_assign(menu, quantity);
}

void Order::removeInList(const Food &menu)
{
    orderList.erase(menu);
}

void Order::makeTakeAway()
{
    takeAway = true;
}

void Order::getDetail()
{
    std::cout << "\nOrder: " << orderNo << std::endl;
    std::cout << "Qty" << std::setw(20) << std::left << "  Menus" << std::setw(8) << std::right << "Amount\n";

    for (auto const &[key, val] : orderList)
    {
        std::cout << std::setw(3) << std::setfill(' ') << val << "  ";
        std::cout << std::setw(17) << std::left << key.name;
        std::cout << std::setw(8) << std::fixed << std::setprecision(2) << std::right << key.price * val << '\n';
    }
}

#endif // ORDER_HPP