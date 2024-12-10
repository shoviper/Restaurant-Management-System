/* 
This folder also contains the database and test cases(settings.json, menu.json, membership.json, 
employee.json, 27052023.json), which will be automatically loaded. However, without these pre-initialized 
files, the program can generate these files with the default value if the json files are not detected. 
*/

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cctype>

#include "order.hpp"
#include "employee.hpp"
#include "membership.hpp"
#include "daily_data.hpp"

int main()
{
    loadSetting();
    loadMenus();
    loadEmployee();
    loadMembership();
    int current_order;
    CurrentDay currentDay;
    std::stringstream file_name;
    try
    {
        file_name.str("");
        file_name << std::setw(2) << std::setfill('0') << ltm->tm_mday << std::setw(2) << std::setfill('0') << (ltm->tm_mon + 1) << ltm->tm_year + 1900 << ".json";
        std::ifstream file(GetCurrentDirectory(file_name.str()));
        if (!file.is_open())
        {
            throw 0;
        }
        currentDay.loadData(file_name.str());
        current_order = currentDay.getOrderNo() + 1;
    }
    catch (...)
    {
        current_order = 1;
    }
    CurrentDay loadDay;

    while (true)
    {
        std::cout << "\nEnter number to select: \n 1. Start a Bill \n 2. Edit Menus \n 3. Settings \n 4. Membership \n 5. Employee Management \n 6. Analysis \n 9. Quit \n> ";
        std::string choice;
        try
        {
            std::getline(std::cin, choice);
            if (choice == "")
            {
                throw 0;
            }
            for (auto &c : choice)
            {
                if (!std::isdigit(c))
                {
                    throw 1;
                }
            }
        }
        catch (int e)
        {
            if (e == 0)
            {
                continue;
            }
            else if (e == 1)
            {
                std::cout << "Invalid Entry. " << std::endl;
                continue;
            }
            else
            {
                continue;
            }
        }

        bool current_choice = true;
        std::string choice_2;
        std::string choice_3;
        auto thisOrder = std::make_unique<Order>(current_order);
        bool discount_applied = false;

        switch (std::stoi(choice))
        {
        case 1:
            std::cout << "\nBilling: #" << current_order << "\n 1. Dine-in \n 2. Take-Away \n 9. Back \n> ";
            try
            {
                std::getline(std::cin, choice);
                if (choice == "2")
                {
                    thisOrder->makeTakeAway();
                    currentDay.addTakeAway();
                }
                else if (choice == "1")
                {
                    ;
                }
                else if (choice == "9")
                {
                    break;
                }
                else
                {
                    throw 0;
                }
            }
            catch (...)
            {
                std::cerr << "Invalid Entry" << std::endl;
                break;
            }

            current_choice = true;
            while (current_choice != false)
            {
                std::cout << "\n 1. Add to/Edit Bill \n 2. Apply Membership \n 3. Preview Bill \n 4. Done \n 9. Back \n> ";
                try
                {
                    std::getline(std::cin, choice);
                    if (choice == "")
                    {
                        throw 0;
                    }
                    for (auto &c : choice)
                    {
                        if (!std::isdigit(c))
                        {
                            throw 1;
                        }
                    }
                }
                catch (int e)
                {
                    if (e == 0)
                    {
                        continue;
                    }
                    else if (e == 1)
                    {
                        std::cout << "Invalid Entry. " << std::endl;
                        continue;
                    }
                    else
                    {
                        continue;
                    }
                }
                switch (std::stoi(choice))
                {
                case 1:
                    for (auto const &[key, val] : Menu)
                    {
                        std::cout << key << std::endl;
                    }
                    std::cout << "\n Enter 0 to quit, Enter the menu's name, then quantity to add to the bill: \n";
                    while (choice != "0")
                    {
                        std::cout << "> ";
                        try
                        {
                            std::getline(std::cin, choice);

                            if (choice == "0")
                            {
                                throw 0;
                            }
                            if (!(Menu.find(choice) != Menu.end()))
                            {
                                throw 1;
                            }
                        }
                        catch (int e)
                        {
                            if (e == 0)
                            {
                                std::cout << "Done." << std::endl;
                            }
                            else if (e == 1)
                            {
                                std::cerr << "This Menu is not currently in the database." << std::endl;
                            }
                            break;
                        }
                        std::cout << "> ";
                        try
                        {
                            std::getline(std::cin, choice_2);
                            if (choice_2 == "")
                            {
                                throw 0;
                            }
                            for (auto &c : choice_2)
                            {
                                if (!std::isdigit(c))
                                {
                                    if (c == '.')
                                    {
                                        throw 1;
                                    }
                                    throw 0;
                                }
                            }
                            thisOrder->addToList(Food(choice, Menu[choice]), std::stoi(choice_2));
                        }
                        catch (int e)
                        {
                            if (e == 0)
                            {
                                std::cerr << "The Entry is not a digit." << std::endl;
                            }
                            else if (e == 1)
                            {
                                std::cerr << "Partial quantity is not permitted." << std::endl;
                            }

                            break;
                        }
                    }

                    thisOrder->calculateTotal();
                    break;

                case 2:
                    if (discount_applied)
                    {
                        std::cerr << "Membership discount is already applied to the bill." << std::endl;
                        continue;
                    }
                    std::cout << "Enter the membership ID: ";
                    try
                    {
                        std::getline(std::cin, choice);
                        for (auto &c : choice)
                        {
                            if (!std::isdigit(c))
                            {
                                throw 0;
                            }
                        }
                        if (!(membership_list.find(choice) != membership_list.end()))
                        {
                            throw 1;
                        }
                        discount_applied = true;
                        thisOrder->setIncome(thisOrder->getIncome() - (thisOrder->getTotal() - membership_list[choice]->calculateDiscount(thisOrder->getTotal())));
                        thisOrder->setTotal(membership_list[choice]->calculateDiscount(thisOrder->getTotal()));
                    }
                    catch (int e)
                    {
                        if (e == 0)
                        {
                            std::cout << "Invalid Entry. " << std::endl;
                        }
                        else if (e == 1)
                        {
                            std::cerr << "This ID is not currently in the database." << std::endl;
                        }
                        break;
                    }
                    break;

                case 3:
                    thisOrder->getDetail();
                    std::cout << "Total: $" << std::fixed << std::setprecision(2) << thisOrder->getTotal() << std::endl;
                    break;

                case 4:
                    current_order += 1;
                    currentDay.addToToday(thisOrder);
                    thisOrder.reset();
                    current_choice = false;
                    break;

                case 9:
                    thisOrder.reset();
                    current_choice = false;
                    break;

                default:
                    std::cerr << "Invalid Entry" << std::endl;
                    break;
                }
            }
            currentDay.saveToday();
            choice.clear();
            break;

        case 2:
            while (current_choice)
            {
                std::cout << "\nMenus: \n";
                std::cout << " 1. Add/Edit Menu \n 2. Remove Menu \n 3. Show all Menus \n 9. Back \n> ";
                try
                {
                    std::getline(std::cin, choice);
                    if (choice == "")
                    {
                        throw 0;
                    }
                    for (auto &c : choice)
                    {
                        if (!std::isdigit(c))
                        {
                            throw 1;
                        }
                    }
                }
                catch (int e)
                {
                    if (e == 0)
                    {
                        continue;
                    }
                    else if (e == 1)
                    {
                        std::cout << "Invalid Entry. " << std::endl;
                        continue;
                    }
                    else
                    {
                        continue;
                    }
                }

                auto choice_price = std::make_unique<std::string>();

                switch (std::stoi(choice))
                {
                case 1:
                    std::cout << "Enter Name: ";
                    std::getline(std::cin, choice);
                    try
                    {
                        std::cout << "Enter Price: ";
                        std::getline(std::cin, *choice_price);
                        for (auto &c : *choice_price)
                        {
                            if (!std::isdigit(c) && c != '.')
                            {
                                throw 0;
                            }
                        }
                        updateMenus(choice, std::stof(*choice_price));
                    }
                    catch (...)
                    {
                        std::cerr << "The Entry is not a digit." << std::endl;
                        break;
                    }
                    break;

                case 2:
                    if (Menu.size() > 1)
                    {
                        std::cout << "\nEnter the menu's name to remove: ";
                        std::getline(std::cin, choice);
                        if (!(Menu.find(choice) != Menu.end()))
                        {
                            std::cerr << "This menu is not currently in the database." << std::endl;
                            break;
                        }
                        removeMenus(choice);
                    }
                    else
                    {
                        std::cerr << "The menu cannot be empty." << std::endl;
                    }
                    break;

                case 3:
                    for (auto const &[key, val] : Menu)
                    {
                        std::cout << key << ": " << val << std::endl;
                    }
                    break;

                case 9:
                    current_choice = false;
                    break;

                default:
                    std::cerr << "Invalid Entry" << std::endl;
                    break;
                }
                choice_price.reset();
            }
            choice.clear();
            break;

        case 3:
            while (current_choice)
            {
                std::cout << "\nSettings: \n";
                std::cout << " 1. Tax rate \n 2. Service Charge \n 3. Tier-1 Membership \n 4. Tier-2 Membership \n 5. Tier-3 Membership \n 9. Back \n> ";
                try
                {
                    std::getline(std::cin, choice);
                    if (choice == "")
                    {
                        throw 0;
                    }
                    for (auto &c : choice)
                    {
                        if (!std::isdigit(c))
                        {
                            throw 1;
                        }
                    }
                }
                catch (int e)
                {
                    if (e == 0)
                    {
                        continue;
                    }
                    else if (e == 1)
                    {
                        std::cout << "Invalid Entry. " << std::endl;
                        continue;
                    }
                    else
                    {
                        continue;
                    }
                }
                switch (std::stoi(choice))
                {
                case 1:
                    try
                    {
                        std::cout << "\n Current Tax rate: " << (config.tax_rate - 1) * 100 << "%\n New value: ";
                        std::getline(std::cin, choice);
                        for (auto &c : choice)
                        {
                            if (!std::isdigit(c) && c != '.')
                            {
                                throw 0;
                            }
                        }
                        setSettings(&config.tax_rate, (std::stod(choice) / 100) + 1);
                    }
                    catch (...)
                    {
                        std::cerr << "The Entry is not a digit." << std::endl;
                        break;
                    }
                    break;

                case 2:
                    try
                    {
                        std::cout << "\n Current Service Charge: " << (config.service_charge - 1) * 100 << "%\n New value: ";
                        std::getline(std::cin, choice);
                        for (auto &c : choice)
                        {
                            if (!std::isdigit(c) && c != '.')
                            {
                                throw 0;
                            }
                        }
                        setSettings(&config.service_charge, (std::stod(choice) / 100) + 1);
                    }
                    catch (...)
                    {
                        std::cerr << "The Entry is not a digit." << std::endl;
                        break;
                    }
                    break;

                case 3:
                    try
                    {
                        std::cout << "\n Current Tier-1 Membership Discount: " << (1 - config.tier_1) * 100 << "%\n New value: ";
                        std::getline(std::cin, choice);
                        for (auto &c : choice)
                        {
                            if (!std::isdigit(c) && c != '.')
                            {
                                throw 0;
                            }
                        }
                        setSettings(&config.tier_1, 1 - std::stod(choice) / 100);
                    }
                    catch (...)
                    {
                        std::cerr << "The Entry is not a digit." << std::endl;
                        break;
                    }
                    break;

                case 4:
                    try
                    {
                        std::cout << "\n Current Tier-2 Membership Discount: " << (1 - config.tier_2) * 100 << "%\n New value: ";
                        std::getline(std::cin, choice);
                        for (auto &c : choice)
                        {
                            if (!std::isdigit(c) && c != '.')
                            {
                                throw 0;
                            }
                        }
                        setSettings(&config.tier_2, 1 - std::stod(choice) / 100);
                    }
                    catch (...)
                    {
                        std::cerr << "The Entry is not a digit." << std::endl;
                        break;
                    }
                    break;

                case 5:
                    try
                    {
                        std::cout << "\n Current Tier-3 Membership Discount: " << (1 - config.tier_3) * 100 << "%\n New value: ";
                        std::getline(std::cin, choice);
                        for (auto &c : choice)
                        {
                            if (!std::isdigit(c) && c != '.')
                            {
                                throw 0;
                            }
                        }
                        setSettings(&config.tier_3, 1 - std::stod(choice) / 100);
                    }
                    catch (...)
                    {
                        std::cerr << "The Entry is not a digit." << std::endl;
                        break;
                    }
                    break;

                case 9:
                    current_choice = false;
                    break;

                default:
                    std::cerr << "Invalid Entry" << std::endl;
                    break;
                }
            }
            choice.clear();
            break;

        case 4:
            while (current_choice)
            {
                std::cout << "\nMembership Management: \n";
                std::cout << " 1. Create Tier-1 \n 2. Create Tier-2 \n 3. Create Tier-3 \n 4. Remove Membership \n 5. Show all Membership \n 9. Back \n> ";

                std::stringstream id;
                try
                {
                    std::getline(std::cin, choice);
                    if (choice == "")
                    {
                        throw 0;
                    }
                    for (auto &c : choice)
                    {
                        if (!std::isdigit(c))
                        {
                            throw 1;
                        }
                    }
                }
                catch (int e)
                {
                    if (e == 0)
                    {
                        continue;
                    }
                    else if (e == 1)
                    {
                        std::cout << "Invalid Entry. " << std::endl;
                        continue;
                    }
                    else
                    {
                        continue;
                    }
                }
                switch (std::stoi(choice))
                {
                case 1:
                    std::cout << "\nEnter Name: ";
                    std::getline(std::cin, choice_2);
                    std::cout << "Enter Surname: ";
                    std::getline(std::cin, choice_3);
                    id << '1' << std::setw(2) << std::setfill('0') << ltm->tm_mday << std::setw(2) << std::setfill('0') << (ltm->tm_mon + 1) << std::to_string(ltm->tm_year + 1900) << std::setw(4) << std::setfill('0') << current_member;
                    membership_list.insert_or_assign(id.str(), std::make_unique<Tier_1>(choice_2, choice_3, id.str()));
                    current_member += 1;
                    saveMembership();
                    break;

                case 2:
                    std::cout << "\nEnter Name: ";
                    std::getline(std::cin, choice_2);
                    std::cout << "Enter Surname: ";
                    std::getline(std::cin, choice_3);
                    id << '2' << std::setw(2) << std::setfill('0') << ltm->tm_mday << std::setw(2) << std::setfill('0') << (ltm->tm_mon + 1) << std::to_string(ltm->tm_year + 1900) << std::setw(4) << std::setfill('0') << current_member;
                    membership_list.insert_or_assign(id.str(), std::make_unique<Tier_2>(choice_2, choice_3, id.str()));
                    current_member += 1;
                    saveMembership();
                    break;

                case 3:
                    std::cout << "\nEnter Name: ";
                    std::getline(std::cin, choice_2);
                    std::cout << "Enter Surname: ";
                    std::getline(std::cin, choice_3);
                    id << '3' << std::setw(2) << std::setfill('0') << ltm->tm_mday << std::setw(2) << std::setfill('0') << (ltm->tm_mon + 1) << std::to_string(ltm->tm_year + 1900) << std::setw(4) << std::setfill('0') << current_member;
                    membership_list.insert_or_assign(id.str(), std::make_unique<Tier_3>(choice_2, choice_3, id.str()));
                    current_member += 1;
                    saveMembership();
                    break;

                case 4:
                    if (membership_list.size() > 1)
                    {
                        ;
                    }
                    else
                    {
                        std::cerr << "The membership cannot be empty." << std::endl;
                        break;
                    }
                    std::cout << "Enter ID to remove: ";
                    try
                    {
                        std::getline(std::cin, choice);
                        for (auto &c : choice)
                        {
                            if (!std::isdigit(c))
                            {
                                throw 0;
                            }
                        }
                        if (!(membership_list.find(choice) != membership_list.end()))
                        {
                            throw 1;
                        }

                        membership_list.erase(choice);
                        saveMembership();
                    }
                    catch (int e)
                    {
                        if (e == 0)
                        {
                            std::cout << "Invalid Entry. " << std::endl;
                        }
                        else if (e == 1)
                        {
                            std::cerr << "This ID is not currently in the database." << std::endl;
                        }
                        break;
                    }
                    break;

                case 5:
                    for (auto const &[key, val] : membership_list)
                    {
                        std::cout << key << ": " << val->getFName() << " " << val->getLName() << std::endl;
                    }
                    break;

                case 9:
                    current_choice = false;
                    break;

                default:
                    std::cerr << "Invalid Entry" << std::endl;
                    break;
                }
            }
            choice.clear();
            break;

        case 5:
            while (current_choice)
            {
                std::cout << "\nEmployee Management: \n";
                std::cout << " 1. Add Employee \n 2. Edit Employee \n 3. Remove Employee \n 4. Show all Employees \n 9. Back \n> ";
                try
                {
                    std::getline(std::cin, choice);
                    if (choice == "")
                    {
                        throw 0;
                    }
                    for (auto &c : choice)
                    {
                        if (!std::isdigit(c))
                        {
                            throw 1;
                        }
                    }
                }
                catch (int e)
                {
                    if (e == 0)
                    {
                        continue;
                    }
                    else if (e == 1)
                    {
                        std::cerr << "Invalid Entry. " << std::endl;
                        continue;
                    }
                    else
                    {
                        continue;
                    }
                }
                auto temp = std::make_unique<std::string[]>(4);
                switch (std::stoi(choice))
                {
                case 1:
                    std::cout << "\nEnter Name: ";
                    std::getline(std::cin, choice);
                    temp[0] = choice;
                    std::cout << "Enter Surname: ";
                    std::getline(std::cin, choice);
                    temp[1] = choice;
                    try
                    {
                        std::cout << "Enter Wages/Hr: ";
                        std::getline(std::cin, choice);
                        for (auto &c : choice)
                        {
                            if (!std::isdigit(c) && c != '.')
                            {
                                throw 0;
                            }
                        }
                        temp[2] = choice;
                    }
                    catch (...)
                    {
                        std::cerr << "The Entry is not a digit." << std::endl;
                        break;
                    }
                    try
                    {
                        std::cout << "Enter Working hours(Enter '8' means full-time): ";
                        std::getline(std::cin, choice);
                        for (auto &c : choice)
                        {
                            if (!std::isdigit(c))
                            {
                                if (c == '.')
                                {
                                    throw 1;
                                }
                                throw 0;
                            }
                        }
                        temp[3] = choice;
                    }
                    catch (int e)
                    {
                        if (e == 0)
                        {
                            std::cerr << "The Entry is not a digit." << std::endl;
                        }
                        else if (e == 1)
                        {
                            std::cerr << "Partial hours is not permitted." << std::endl;
                        }

                        break;
                    }

                    if (temp[3] == "8")
                    {
                        employee_list.push_back(std::make_unique<Employee>(temp[0], temp[1], current_id, std::stod(temp[2])));
                    }
                    else
                    {
                        employee_list.push_back(std::make_unique<Part_time>(temp[0], temp[1], current_id, std::stod(temp[2]), std::stoi(temp[3])));
                    }
                    saveEmployee();
                    current_id += 1;
                    temp.reset();
                    break;

                case 2:
                    std::cout << std::left << std::setw(3) << "# " << std::setw(20) << "Name - Surname" << std::endl;
                    for (size_t i = 0; i < employee_list.size(); i++)
                    {
                        if (employee_list[i]->getHours() == 8)
                        {
                            std::cout << std::left << std::setw(3) << i << "*";
                        }
                        else
                        {
                            std::cout << std::left << std::setw(3) << i;
                        }
                        std::cout << std::setw(20) << employee_list[i]->getEmployee() << std::endl;
                    }

                    try
                    {
                        std::cout << "You cannot change the full-time employees(*) working hours.\nEnter # to Select: ";
                        std::getline(std::cin, choice);
                        if (std::stoul(choice) >= employee_list.size())
                        {
                            throw 0;
                        }
                        std::cout << "Enter 1 to set wage, Enter 2 to set working hours of " << employee_list[std::stoi(choice)]->getEmployee() << "\n> ";
                        std::getline(std::cin, choice_2);
                        if (choice_2 == "1")
                        {
                            std::cout << "Enter new wage: ";
                            std::getline(std::cin, choice_2);
                            employee_list[std::stoi(choice)]->setWages(std::stod(choice_2));
                        }
                        else if (choice_2 == "2")
                        {
                            std::cout << "Enter new working hours: ";
                            std::getline(std::cin, choice_2);
                            employee_list[std::stoi(choice)]->setHours(std::stod(choice_2));
                        }
                        saveEmployee();
                    }
                    catch (...)
                    {
                        std::cerr << "Invalid Entry" << std::endl;
                    }

                    break;

                case 3:
                    if (employee_list.size() > 1)
                    {
                        ;
                    }
                    else
                    {
                        std::cerr << "The membership cannot be empty." << std::endl;
                        break;
                    }
                    std::cout << std::left << std::setw(5) << "ID " << std::setw(20) << "Name - Surname" << std::endl;
                    for (auto const &e : employee_list)
                    {
                        std::cout << std::left << std::setw(5) << e->getID() << std::setw(20) << e->getEmployee() << std::endl;
                    }
                    std::cout << "Enter ID to Select: ";
                    std::getline(std::cin, choice);

                    try
                    {
                        for (auto &c : choice)
                        {
                            if (!std::isdigit(c))
                            {
                                throw 0;
                            }
                        }

                        if (emp_map.find(choice) != emp_map.end())
                        {
                            emp_map.erase(choice);
                        }
                        else
                        {
                            throw 1;
                        }
                        updateEmployee();
                    }
                    catch (int e)
                    {
                        if (e == 0)
                        {
                            std::cerr << "The Entry is not a digit." << std::endl;
                        }
                        else if (e == 1)
                        {
                            std::cerr << "The Entry does not exist." << std::endl;
                        }
                        else
                        {
                            std::cerr << "Unknown exception." << std::endl;
                        }
                    }
                    catch (...)
                    {
                        std::cerr << "Unknown exception." << std::endl;
                    }

                    break;

                case 4:
                    std::cout << std::left << std::setw(5) << "ID " << std::setw(20) << "Name - Surname" << std::left << std::setw(5) << "Hours"
                              << std::setw(6) << "  Wages         " << std::setw(10) << " Salary" << std::endl;
                    for (auto const &e : employee_list)
                    {
                        std::cout << std::left << std::setw(5) << e->getID() << std::setw(20) << e->getEmployee()
                                  << e->getHours() << " hrs"
                                  << "  $" << std::setw(5) << std::setprecision(2) << std::fixed << e->getWages() << "/per hr"
                                  << "  $" << std::setprecision(2) << std::fixed << e->getSalary() << std::endl;
                    }
                    break;

                case 9:
                    current_choice = false;
                    break;

                default:
                    std::cerr << "Invalid Entry" << std::endl;
                    break;
                }
            }

            choice.clear();
            break;

        case 6:
            std::cout << "\nAnalysis: ";
            while (current_choice)
            {
                std::cout << "\n 1. Today's Summary \n 2. Others' day Summary \n 9. Back \n> ";
                try
                {
                    std::getline(std::cin, choice);
                    if (choice == "")
                    {
                        throw 0;
                    }
                    for (auto &c : choice)
                    {
                        if (!std::isdigit(c))
                        {
                            throw 1;
                        }
                    }
                }
                catch (int e)
                {
                    if (e == 0)
                    {
                        continue;
                    }
                    else if (e == 1)
                    {
                        std::cout << "Invalid Entry. " << std::endl;
                        continue;
                    }
                    else
                    {
                        continue;
                    }
                }
                switch (std::stoi(choice))
                {
                case 1:
                    currentDay.summary();
                    break;

                case 2:
                    std::cout << "Enter date: ";
                    std::getline(std::cin, choice);
                    std::cout << "Enter month: ";
                    std::getline(std::cin, choice_2);
                    std::cout << "Enter year: ";
                    std::getline(std::cin, choice_3);

                    file_name.str("");
                    file_name << std::setw(2) << std::setfill('0') << choice << std::setw(2) << std::setfill('0') << choice_2 << choice_3 << ".json";
                    try
                    {
                        if (loadDay.loadData(file_name.str()))
                        {
                            throw 0;
                        }
                    }
                    catch (...)
                    {
                        loadDay.loadData(file_name.str());
                        loadDay.summary();
                    }
                    break;

                case 9:
                    current_choice = false;
                    break;

                default:
                    std::cerr << "Invalid Entry" << std::endl;
                    break;
                }
            }

            choice.clear();
            choice_2.clear();
            choice_3.clear();
            break;

        case 9:
            std::cout << "\nClosing...\n";
            return 0;

        default:
            std::cerr << "Invalid Entry" << std::endl;
            break;
        }
    }

    return 0;
}