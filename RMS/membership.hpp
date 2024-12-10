#ifndef MEMBERSHIP_HPP
#define MEMBERSHIP_HPP

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

#include "configure.hpp"

class Membership
{
protected:
    std::string first_name, last_name, id;

public:
    Membership() = delete;
    explicit Membership(std::string fname, std::string lname, std::string i) : first_name(fname), last_name(lname), id(i) {}
    virtual ~Membership() {}

    virtual double calculateDiscount(double b) = 0;
    std::string getFName();
    std::string getLName();
};

std::string Membership::getFName()
{
    return first_name;
}
std::string Membership::getLName()
{
    return last_name;
}

class Tier_1 : public Membership
{
public:
    explicit Tier_1(std::string fname, std::string lname, std::string i) : Membership(fname, lname, i) {}
    ~Tier_1() {}

    double calculateDiscount(double b) override;
};

double Tier_1::calculateDiscount(double b)
{
    return config.tier_1 * b;
}

class Tier_2 : public Membership
{
public:
    explicit Tier_2(std::string fname, std::string lname, std::string i) : Membership(fname, lname, i) {}
    ~Tier_2() {}

    double calculateDiscount(double b) override;
};
double Tier_2::calculateDiscount(double b)
{
    return config.tier_2 * b;
}

class Tier_3 : public Membership
{
public:
    explicit Tier_3(std::string fname, std::string lname, std::string i) : Membership(fname, lname, i) {}
    ~Tier_3() {}

    double calculateDiscount(double b) override;
};

double Tier_3::calculateDiscount(double b)
{
    return config.tier_3 * b;
}

#endif // MEMBERSHIP_HPP