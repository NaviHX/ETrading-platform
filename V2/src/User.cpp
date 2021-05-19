#include "User.h"
#include <iostream>

User::User(
    const std::string &n,
    const std::string &pw,
    const double b)
    : name(n), password(pw), balance(b)
{
}

bool Consumer::addCart(std::string comId, int num)
{
    bool added = false;
    for (auto& it : cart)
    {
        if (comId.compare(it.first) == 0)
        {
            it.second += num;
            added = true;
        }
    }
    if (!added)
    {
        cart.push_back(std::pair<std::string, int>(comId, num));
        added = true;
    }
    return added;
}

Consumer::Consumer(const std::string &n, const std::string &pw, const double b) : User(n, pw, b)
{
}

Merchant::Merchant(const std::string &n, const std::string &pw, const double b) : User(n, pw, b)
{
}
