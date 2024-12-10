#ifndef CONFIGURE_HPP
#define CONFIGURE_HPP

#include <fstream>
#include <vector>
#include <string>
#include <map>

#include "Windows.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// Getting the earlier directory of the exe file, because the exe file is in build folder
std::string GetCurrentDirectory(std::string fname)
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");

    return std::string(buffer).substr(0, pos - 5) + fname;
}

// Initialize the config value
const double defaultVal = 1.0;

struct Config
{
    double tax_rate;
    double service_charge;
    double tier_1, tier_2, tier_3;
};

Config config;

// Load settings from Json
void loadSetting()
{
    try
    {

        std::ifstream settingsFile(GetCurrentDirectory("settings.json"));
        if (!settingsFile.is_open())
        {
            throw 0;
        }
        json settingsJson = json::parse(settingsFile);

        config.tax_rate = settingsJson["tax_rate"];
        config.service_charge = settingsJson["service_charge"];
        config.tier_1 = settingsJson["tier_1"];
        config.tier_2 = settingsJson["tier_2"];
        config.tier_3 = settingsJson["tier_3"];
    }
    catch (...)
    {
        json j = {
            {"tax_rate", defaultVal},
            {"service_charge", defaultVal},
            {"tier_1", defaultVal},
            {"tier_2", defaultVal},
            {"tier_3", defaultVal}};
        std::ofstream updateSettingsFile(GetCurrentDirectory("settings.json"));
        updateSettingsFile << j.dump(4);

        std::cout << "- created settings.json" << std::endl;
        config.tax_rate = defaultVal;
        config.service_charge = defaultVal;
        config.tier_1 = defaultVal;
        config.tier_2 = defaultVal;
        config.tier_3 = defaultVal;
    }
}

// Save settings to Json
void saveSettings()
{
    json j = {
        {"tax_rate", config.tax_rate},
        {"service_charge", config.service_charge},
        {"tier_1", config.tier_1},
        {"tier_2", config.tier_2},
        {"tier_3", config.tier_3}};
    std::ofstream updateSettingsFile(GetCurrentDirectory("settings.json"));
    updateSettingsFile << j.dump(4);
}

// Template for setting the config properties
template <typename T>
void setSettings(T *s, double d)
{
    *s = d;
    saveSettings();
}

// Initialize map for Menu
std::map<std::string, float> Menu;

// Load Menu from Json
void loadMenus()
{
    try
    {
        std::ifstream menuFile(GetCurrentDirectory("menu.json"));
        if (!menuFile.is_open())
        {
            throw 0;
        }
        json menuJson = json::parse(menuFile);
        Menu = menuJson;
    }
    catch (...)
    {
        json j;
        j["default"] = defaultVal;

        std::ofstream updateMenuFile(GetCurrentDirectory("menu.json"));
        updateMenuFile << j.dump(4);
    }
}

// Save menu to Json
void saveMenus()
{
    json j;
    for (auto const &[key, val] : Menu)
    {
        j[key] = val;
    }

    std::ofstream updateMenuFile(GetCurrentDirectory("menu.json"));
    updateMenuFile << j.dump(4);
}

// Update Menu
void updateMenus(std::string &s, float &&f)
{
    Menu.insert_or_assign(s, f);
    saveMenus();
}

// Remove Menu
void removeMenus(std::string &s)
{
    Menu.erase(s);
    saveMenus();
}

#include "employee.hpp"
#include <memory>

// Initialize Employee
std::map<std::string, std::vector<std::string>> emp_map;
std::vector<std::unique_ptr<Employee>> employee_list;
int current_id = 1;

// save Employee (vector -> map)
void saveEmployee()
{
    json j;
    for (auto &e : employee_list)
    {
        j[std::to_string(e->getID())] = {e->getName(), e->getSur(), std::to_string(e->getWages()), std::to_string(e->getHours())};

        std::vector<std::string> string_vector;
        string_vector.push_back(e->getName());
        string_vector.push_back(e->getSur());
        string_vector.push_back(std::to_string(e->getWages()));
        string_vector.push_back(std::to_string(e->getHours()));

        emp_map.insert_or_assign(std::to_string(e->getID()), string_vector);
    }

    std::ofstream updateEmployeeFile(GetCurrentDirectory("employee.json"));
    updateEmployeeFile << j.dump(4);
}

// load Employee (map -> vector)
void loadEmployee()
{
    try
    {
        std::ifstream empFile(GetCurrentDirectory("employee.json"));
        if (!empFile.is_open())
        {
            throw 0;
        }
        json empJson = json::parse(empFile);
        emp_map = empJson;
        employee_list.clear();
        for (auto const &[key, val] : emp_map)
        {
            if (current_id < std::stoi(key))
            {
                current_id = std::stoi(key);
                current_id += 1;
            }

            if (val[3] == "8")
            {
                employee_list.push_back(std::make_unique<Employee>(val[0], val[1], std::stoi(key), std::stod(val[2])));
            }
            else
            {
                employee_list.push_back(std::make_unique<Part_time>(val[0], val[1], std::stoi(key), std::stod(val[2]), std::stoi(val[3])));
            }
        }
        saveEmployee();
    }
    catch (...)
    {
        json j;
        j["0"] = {"first_name", "last_name", std::to_string(defaultVal), std::to_string(defaultVal)};
        std::vector<std::string> string_vector;
        string_vector.push_back("last_name");
        string_vector.push_back("first_name");
        string_vector.push_back(std::to_string(defaultVal));
        string_vector.push_back(std::to_string(defaultVal));

        emp_map.insert_or_assign(std::to_string(0), string_vector);

        std::ofstream updateEmployeeFile(GetCurrentDirectory("employee.json"));
        updateEmployeeFile << j.dump(4);
    }
}

// update Employee
void updateEmployee()
{
    employee_list.clear();
    for (auto const &[key, val] : emp_map)
    {
        if (current_id < std::stoi(key))
        {
            current_id = std::stoi(key);
            current_id += 1;
        }

        if (val[3] == "8")
        {
            employee_list.push_back(std::make_unique<Employee>(val[0], val[1], std::stoi(key), std::stod(val[2])));
        }
        else
        {
            employee_list.push_back(std::make_unique<Part_time>(val[0], val[1], std::stoi(key), std::stod(val[2]), std::stoi(val[3])));
        }
    }
    saveEmployee();
}

// Memebership Initialize
#include "membership.hpp"

time_t now = time(0);
tm *ltm = localtime(&now);
int current_member = 1;

std::map<std::string, std::vector<std::string>> mem_map;
std::map<std::string, std::unique_ptr<Membership>> membership_list;

// Save membership to Json
void saveMembership()
{
    json j;
    for (auto const &[key, val] : membership_list)
    {
        j[key] = {val->getFName(), val->getLName()};
    }

    std::ofstream updateMembership(GetCurrentDirectory("membership.json"));
    updateMembership << j.dump(4);
}

// Load membership to Json
void loadMembership()
{
    std::stringstream date;
    date << std::setw(2) << std::setfill('0') << ltm->tm_mday << std::setw(2) << std::setfill('0') << (ltm->tm_mon + 1) << std::to_string(ltm->tm_year + 1900);
    try
    {

        std::ifstream memFile(GetCurrentDirectory("membership.json"));
            if (!memFile.is_open())
            {
                throw 0;
            }
        json memJson = json::parse(memFile);
        mem_map = memJson;
    }
    catch (...)
    {
            json j;
            j["1000000000000"] = {"first_name", "last_name"};

            std::ofstream updateMemFile(GetCurrentDirectory("membership.json"));
            updateMemFile << j.dump(4);
    }
    for (auto const &[key, val] : mem_map)
    {
        if (key[0] == '1')
        {
            membership_list.insert_or_assign(key, std::make_unique<Tier_1>(val[0], val[1], key));
        }
        else if (key[0] == '2')
        {
            membership_list.insert_or_assign(key, std::make_unique<Tier_2>(val[0], val[1], key));
        }
        else if (key[0] == '3')
        {
            membership_list.insert_or_assign(key, std::make_unique<Tier_3>(val[0], val[1], key));
        }

        /*  
        Representation of each digits, example:
         1     26052003    0001
        tier  dd/mm/yyy     no
        */
        if (key.substr(1, 8) == date.str())
        {
            current_member = std::stoi(key.substr(9, 12)) + 1;
        }
    }
}

#endif // CONFIGURE_HPP