#ifndef MENU_HPP
#define MENU_HPP

#include <string>
#include <sstream>
#include <limits>

struct Food
{
    std::string name;
    float price;

    Food() : name(""), price(0.0) {}
    Food(std::string n, float p) : name(n), price(p) {}

    friend bool operator<(const Food &l, const Food &r)
    {
        return l.price < r.price;
    }
};

#endif // MENU_HPP