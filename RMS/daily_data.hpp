#ifndef DAILY_DATA_HPP
#define DAILY_DATA_HPP

#include <string>
#include <sstream>
#include <ctime>

#include "order.hpp"
#include "configure.hpp"

class CurrentDay
{
private:
    std::vector<std::unique_ptr<Order>> order_list;
    double todayIncome = 0.0;
    double todayTotal = 0.0;
    int takeAwayNo = 0;

public:
    explicit CurrentDay() {}
    ~CurrentDay() {}

    void addToToday(std::unique_ptr<Order> &order);
    void addTakeAway();
    int getTakeAway();
    int getOrderNo();

    void summary();

    void saveToday();
    bool loadData(std::string &&d);
};

void CurrentDay::addToToday(std::unique_ptr<Order> &order)
{
    order_list.push_back(std::move(order));
    if (order_list.back())
        todayTotal += order_list.back()->getTotal();
    todayIncome += order_list.back()->getIncome();
}

void CurrentDay::addTakeAway()
{
    takeAwayNo += 1;
}

int CurrentDay::getTakeAway()
{
    return takeAwayNo;
}

int CurrentDay::getOrderNo()
{
    return order_list.back()->getOrderNo();
}

void CurrentDay::summary()
{
    auto tallies = std::make_unique<std::map<std::string, int>>();

    std::cout << "\nStatistical Analysis View: \n"
              << std::endl;
    std::cout << "Total Order: " << getOrderNo() << std::endl;
    std::cout << "Amount of Take Away Order: " << getTakeAway() << std::endl;
    std::cout << std::endl
              << '~'
              << std::setw(3) << std::setprecision(0) << std::fixed << 100.0 * (float(getOrderNo() - getTakeAway()) / float(getOrderNo())) << "% of the customers DINE-IN" << std::endl;
    std::cout << '~' << std::setw(3) << std::setprecision(0) << std::fixed << 100.0 * (float(getTakeAway()) / float(getOrderNo())) << "% of the customers TAKE-AWAY" << std::endl;

    std::cout << '\n'
              << "Average Ticket Size: " << std::setprecision(2) << std::fixed << todayTotal / double(getOrderNo()) << std::endl;
    std::cout
        << "Average Profit per Ticket Size: " << std::setprecision(2) << std::fixed << todayIncome / double(getOrderNo()) << std::endl;

    std::cout << "\nBilling Summary" << std::endl;
    std::cout << std::setw(5) << "Order" << std::setw(17) << std::right << "Amount\n";
    for (const auto &order : order_list)
    {
        std::cout << std::setw(4) << std::setfill('0') << order->getOrderNo() << ": ";
        std::cout << std::fixed << std::setprecision(2) << std::setw(15) << std::setfill(' ') << std::right << order->getTotal() << std::endl;

        for (auto const &[key, val] : order->getOrderList())
        {
            if (tallies->find(key.name) != tallies->end())
            {
                tallies->insert_or_assign(key.name, tallies->at(key.name) + val);
                continue;
            }
            tallies->insert({key.name, val});
        }
    }
    std::cout << '\n'
              << std::setw(15) << "Total: " << todayTotal << std::endl;
    std::cout << std::setw(15) << "After Tax: " << todayIncome << std::endl;

    std::cout << "\nOrder Tallies: " << std::endl;
    for (auto const &[key, val] : Menu)
    {
        if (tallies->find(key) != tallies->end())
        {
            std::cout << std::setw(15) << std::right << key << std::setw(4) << std::right << tallies->at(key) << "|";
            for (size_t i = 0; i < (size_t)tallies->at(key); i++)
            {
                std::cout << "[]";
            }
            std::cout << std::endl;
            continue;
        }

        std::cout << std::setw(15) << std::right << key << std::setw(4) << std::right << "0"
                  << "|" << std::endl;
    }
}

void CurrentDay::saveToday()
{
    json j;
    json jsonObj = json::array();
    for (auto &o : order_list)
    {
        jsonObj.clear();
        for (auto const &[key, val] : o->getOrderList())
        {
            jsonObj.push_back(key.name);
            jsonObj.push_back(std::to_string(key.price));
            jsonObj.push_back(std::to_string(val));
        }
        j[std::to_string(o->getOrderNo())] = {std::to_string(o->getTakeAway()),
                                              std::to_string(o->getTotal()),
                                              std::to_string(o->getIncome()),
                                              jsonObj};
    }
    std::stringstream file_name;
    file_name << std::setw(2) << std::setfill('0') << ltm->tm_mday << std::setw(2) << std::setfill('0') << (ltm->tm_mon + 1) << ltm->tm_year + 1900 << ".json";
    std::ofstream updateMenuFile(GetCurrentDirectory(file_name.str()));
    updateMenuFile << j.dump(4);
}

bool CurrentDay::loadData(std::string &&d)
    {
        std::ifstream file(GetCurrentDirectory(d));
        if (!file.is_open())
        {
            std::cerr << "Failed to open the JSON file." << std::endl;
            return false;
        }

        json j;
        try
        {
            file >> j;
        }
        catch (const json::exception &e)
        {
            std::cerr << "Failed to parse the JSON file: " << e.what() << std::endl;
            return false;
        }

        order_list.clear();
        takeAwayNo = 0;
        todayTotal = 0;
        todayIncome = 0;

        for (const auto &[key, value] : j.items())
        {
            int orderNo = std::stoi(key);
            int takeAway = std::stoi(value[0].get<std::string>());
            double total = std::stod(value[1].get<std::string>());
            double income = std::stod(value[2].get<std::string>());

            takeAwayNo += takeAway;
            todayTotal += total;
            todayIncome += income;

            json jsonObj = value[3];
            std::map<Food, int> orderList;

            for (size_t i = 0; i < jsonObj.size(); i += 3)
            {
                std::string name = jsonObj[i].get<std::string>();
                double price = std::stod(jsonObj[i + 1].get<std::string>());
                int quantity = std::stoi(jsonObj[i + 2].get<std::string>());

                Food orderKey{name, float(price)};
                orderList[orderKey] = quantity;
            }

            auto order = std::make_unique<Order>(orderNo);
            if (takeAway == 1)
            {
                order->makeTakeAway();
            }
            order->setOrderList(orderList);
            order->setTotal(total);
            order->setIncome(income);

            order_list.push_back(std::move(order));
        }

        file.close();

        return true;
    }

#endif // MENU_HPP