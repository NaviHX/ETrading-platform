#include "Commidity.h"

double Book::discount = 1.0;
double Food::discount = 1.0;
double Cloth::discount = 1.0;

Commdity::Commdity(
    double pr,
    double pe,
    int q,
    const std::string &ct,
    const std::string &n,
    const std::string &own,
    const std::string &desc)
    : price(pr), percent(pe), quantity(q), comType(ct), name(n), owner(own), description(desc)
{
}

Book::Book(double pr, double pe, int q, const std::string &ct, const std::string &n, const std::string &own, const std::string &desc) : Commdity(pr, pe, q, ct, n, own, desc)
{
}

Food::Food(double pr, double pe, int q, const std::string &ct, const std::string &n, const std::string &own, const std::string &desc) : Commdity(pr, pe, q, ct, n, own, desc)
{
}

Cloth::Cloth(double pr, double pe, int q, const std::string &ct, const std::string &n, const std::string &own, const std::string &desc) : Commdity(pr, pe, q, ct, n, own, desc)
{
}

bool Commdity::buy(int count)
{
    if (count <= quantity)
    {
        quantity -= count;
        return true;
    }
    return false;
}

bool Commdity::isType(const std::string &t)
{
    if (comType.compare(t) == 0)
        return true;
    return false;
}
