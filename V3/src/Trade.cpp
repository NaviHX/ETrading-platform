#include <iostream>
#include <fstream>
#include "Trade.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <sstream>
#include <strstream>

bool Trade::readUserFile(bool quiet, const std::string &fp)
{
    if (!quiet)
        std::cout << "Reading user data file : " << fp << " ... ";
    std::ifstream f(fp, std::ios::in);
    if (!f.good())
    {
        if (!quiet)
        {
            std::cout << fp << ": not exist\n";
            std::string temp;
            std::cout << "Continue? (Y/n)";
            std::cin >> temp;
            if (temp[0] == 'y' || temp[0] == 'Y')
                return false;
            else
            {
                exit(0);
            }
        }
        return false;
    }
    std::string tempname, temppassword;
    int temptype, tempbal;
    f >> tempname;
    while (!f.eof())
    {
        f >> temppassword >> temptype >> tempbal;
        User *u = nullptr;
        switch (temptype)
        {
        case 0:
            u = new Consumer(tempname, temppassword, tempbal);
            break;
        case 1:
            u = new Merchant(tempname, temppassword, tempbal);
            break;
        }
        if (u != nullptr)
        {
            userList.push_back(u);
        }
        f >> tempname;
    }
    f.close();
    if (!quiet)
        std::cout << "Done\n";
    return true;
}

bool Trade::saveUserFile(bool quiet, const std::string &fp) const
{
    if (!quiet)
        std::cout << "Saving user data file : " << fp << " ... ";
    std::ofstream f(fp, std::ios::out);
    for (auto it : userList)
    {
        User::Type t = it->getUserType();
        int type;
        if (it->getName().compare(adminName) == 0)
        {
            continue;
        }
        switch (t)
        {
        case User::Type::consumer:
            type = 0;
            break;
        case User::Type::merchant:
            type = 1;
            break;
        }
        f << it->getName()
          << " " << it->getPassword()
          << " " << type
          << " " << it->getBalance() << std::endl;
    }
    f.close();
    if (!quiet)
        std::cout << "Done\n";
    return true;
}

bool Trade::readCommFile(bool quiet, const std::string &fp)
{
    if (!quiet)
        std::cout << "Reading commdity data file : " << fp << " ... ";
    std::ifstream f(fp, std::ios::in);
    if (!f.good())
    {
        std::cout << fp << ": not exist\n";
        std::string temp;
        std::cout << "Continue? (Y/n)";
        std::cin >> temp;
        if (temp[0] == 'y' || temp[0] == 'Y')
            return false;
        else
        {
            exit(0);
        }
    }
    double bd, fd, cd;
    f >> bd >> fd >> cd;
    Book::setDiscount(bd);
    Food::setDiscount(fd);
    Cloth::setDiscount(cd);

    std::string tempname, temptype, tempowner, desc;
    double tempprice, temppercent;
    int quantity;
    f >> tempname;
    while (!f.eof())
    {
        f >> temptype >> tempowner >> tempprice >> temppercent >> quantity >> desc;
        auto it = commMap.find(temptype);
        auto ty = it->second;
        Commdity *c = nullptr;
        switch (ty)
        {
        case book:
            c = new Book(tempprice, temppercent, quantity, temptype, tempname, tempowner, desc);
            break;

        case food:
            c = new Food(tempprice, temppercent, quantity, temptype, tempname, tempowner, desc);
            break;

        case cloth:
            c = new Cloth(tempprice, temppercent, quantity, temptype, tempname, tempowner, desc);
            break;
        }
        // Commdity c(tempprice, temppercent, quantity, temptype, tempname, tempowner);
        commList.push_back(c);
        f >> tempname;
    }
    f.close();
    if (!quiet)
        std::cout << "Done\n";
    return true;
}

bool Trade::saveCommFile(bool quiet, const std::string &fp) const
{
    if (!quiet)
        std::cout << "Saving commdity data file : " << fp << " ... ";
    std::ofstream f(fp, std::ios::out);
    f << Book::getDiscount() << " " << Food::getDiscount() << " " << Cloth::getDiscount() << std::endl;
    for (const auto &it : commList)
    {
        f << it->getName()
          << " " << it->getComType()
          << " " << it->getOwner()
          << " " << it->getOriginPrice()
          << " " << it->getPercent()
          << " " << it->getQuantity()
          << " " << it->getDesc() << std::endl;
        // delete it;
    }
    f.close();
    if (!quiet)
        std::cout << "Done\n";
    return true;
}

bool Trade::haveUser(const std::string &username) const
{
    for (auto it : userList)
    {
        if (username.compare(it->getName()) == 0)
        {
            return true;
        }
    }
    return false;
}

bool Trade::checkPassword(const std::string &username,
                          const std::string &password) const
{
    for (auto it : userList)
    {
        if (username.compare(it->getName()) == 0)
        {
            if (password.compare(it->getPassword()) == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return false;
}

bool Trade::addUser(const std::string &username, const std::string &password, int type)
{
    if (haveUser(username))
    {
        return false;
    }

    User *u = nullptr;
    switch (type)
    {
    case 0:
        u = new Consumer(username, password, 0);
        break;
    case 1:
        u = new Merchant(username, password, 0);
        break;
    }
    if (u != nullptr)
    {
        userList.push_back(u);
        return true;
    }
    return false;
}

bool Trade::delUser(const std::string &username)
{
    for (auto it = userList.begin(); it != userList.end(); it++)
    {
        if (username.compare((*it)->getName()) == 0)
        {
            User *temp = *it;
            userList.erase(it);
            delete temp;
            return true;
        }
    }
    return false;
}

std::string Trade::getUserInfo(const std::string &username) const
{
    std::string ret;
    char buff[MAXBUF];
    memset(buff, 0, MAXBUF);
    std::ostrstream oss(buff, MAXBUF);
    if (username == "*" || haveUser(username))
    {
        for (auto it : userList)
        {
            if (username == "*" || username.compare(it->getName()) == 0)
            {
                // 输出用户信息

                oss << "User Info [ " << it->getName() << " ]" << std::endl;
                oss << "Type : " << (int)it->getUserType() << std::endl;
                oss << "Balance : " << it->getBalance() << std::endl;
            }
        }
    }
    ret = buff;
    if (ret.size() == 0)
    {
        oss << "CANNOT find user : " << username << std::endl;
    }
    return ret;
}

std::string Trade::listComm() const
{
    std::string ret;
    char buff[MAXBUF];
    memset(buff, 0, MAXBUF);
    std::ostrstream oss(buff, MAXBUF);
    for (const auto &it : commList)
    {
        oss << "Name : " << it->getName() << " "
            << "Price : " << it->getPrice() << " "
            << "Type : " << it->getComType() << " "
            << "Quantity : " << it->getQuantity() << " "
            << "Discount : " << it->getRealDiscount() << " "
            << "Owner : " << it->getOwner() << " "
            << "Description : " << it->getDesc() << std::endl;
    }
    ret = buff;
    return ret;
}

std::string Trade::listComm(const std::string &name, const std::string &comType, const std::string &uname) const
{
    std::string ret;
    char buff[MAXBUF];
    memset(buff, 0, MAXBUF);
    std::ostrstream oss(buff, MAXBUF);
    for (const auto &it : commList)
    {
        if ((name == "*" || it->getName().find(name) != std::string::npos) && (comType == "*" || comType.compare(it->getComType()) == 0) && (uname == "" || uname.compare(it->getOwner()) == 0))
        {
            oss << "Name : " << it->getName() << " "
                << "Price : " << it->getPrice() << " "
                << "Type : " << it->getComType() << " "
                << "Quantity : " << it->getQuantity() << " "
                << "Discount : " << it->getRealDiscount() << " "
                << "Owner : " << it->getOwner() << " "
                << "Description : " << it->getDesc() << std::endl;
        }
    }
    ret = buff;
    return ret;
}

bool Trade::addComm(const std::string &name, const std::string &uname, const std::string &comType, double price, const std::string &desc)
{
    if (price < 0)
    {
        return false;
    }
    for (auto it : commList)
    {
        if (name.compare(it->getName()) == 0)
            return false;
    }
    auto it = commMap.find(comType);
    if (it == commMap.end())
    {
        return false;
    }
    Commdity *c = nullptr;
    switch (it->second)
    {
    case book:
        c = new Book(price, 1, 0, comType, name, uname, desc);
        break;
    case food:
        c = new Food(price, 1, 0, comType, name, uname, desc);
        break;
    case cloth:
        c = new Cloth(price, 1, 0, comType, name, uname, desc);
        break;
    }
    // Commdity c(price, 1, 0, comType, name, uname);
    commList.push_back(c);
    return true;
}

bool Trade::delComm(const std::string &name, const std::string &uname)
{
    for (auto it = commList.begin(); it != commList.end(); it++)
    {
        if (name.compare((*it)->getName()) == 0 && uname.compare((*it)->getOwner()) == 0)
        {
            commList.erase(it);
            return true;
        }
    }
    return false;
}

bool Trade::changeQuantity(const std::string &name, const std::string &uname, int q)
{
    if (q < 0)
    {
        return false;
    }
    for (auto it = commList.begin(); it != commList.end(); it++)
    {
        if (name.compare((*it)->getName()) == 0 && (uname == adminName || uname.compare((*it)->getOwner()) == 0))
        {
            (*it)->setQuantity(q);
            return true;
        }
    }
    return false;
}

bool Trade::setPrice(const std::string &name, const std::string &uname, double p)
{
    if (p < 0)
    {
        return false;
    }
    for (auto it = commList.begin(); it != commList.end(); it++)
    {
        if (name.compare((*it)->getName()) == 0 && uname.compare((*it)->getOwner()) == 0)
        {
            (*it)->setOriginPrice(p);
            return true;
        }
    }
    return false;
}

bool Trade::setPercent(const std::string &name, const std::string &uname, double p)
{
    if (p < 0)
    {
        return false;
    }
    for (auto it = commList.begin(); it != commList.end(); it++)
    {
        if (name.compare((*it)->getName()) == 0 && (uname.compare((*it)->getOwner()) == 0 || uname == adminName))
        {
            (*it)->setPercent(p);
            return true;
        }
    }
    return false;
}

bool Trade::setPercent(double p, const std::string &type, const std::string &uname)
{
    if (p < 0)
    {
        return false;
    }
    /* 
    for (auto it = commList.begin(); it != commList.end(); it++)
    {
        if (type.compare((*it)->getComType()) == 0 && (uname.compare((*it)->getOwner()) == 0 || uname == adminName))
        {
            (*it)->setPercent(p);
        }
    }
    return true;
     */
    if (uname != adminName)
    {
        return false;
    }
    auto it = commMap.find(type);
    if (it == commMap.end())
    {
        return false;
    }
    auto ty = it->second;
    switch (ty)
    {
    case book:
        Book::setDiscount(p);
        break;

    case food:
        Food::setDiscount(p);
        break;

    case cloth:
        Cloth::setDiscount(p);
        break;
    }
    return true;
}

std::string Trade::getOwner(const std::string &name)
{
    for (auto &cit : commList)
    {
        if (cit->getName() == name)
        {
            return cit->getOwner();
        }
    }
    return "";
}

bool Trade::buy(const std::string &uname)
{
    for (auto uit : userList)
    {
        if (uname.compare(uit->getName()) == 0 && uit->getUserType() == User::Type::consumer)
        {
            auto cit = dynamic_cast<Consumer *>(uit);

            if (cit->haveOrder == false)
                return false;

            double sum = dynamic_cast<Consumer *>(uit)->sum;
            auto order = dynamic_cast<Consumer *>(uit)->order;

            /*
            for (auto cit : cart)
            {
                double price = getPrice(cit.first);
                if (price == -1 && cit.second <= getQuantity(cit.first))
                {
                    return false;
                }
                changeQuantity(cit.first, uit->getName(), getQuantity(cit.first) - cit.second);
                sum += price * cit.second;
            }
            */
            if (sum > uit->getBalance())
            {
                return false;
            }

            for (auto itemPair : order)
            {
                auto owner = getOwner(itemPair.first);
                addbal(owner, getPrice(itemPair.first) * itemPair.second);
            }

            uit->setBalance(uit->getBalance() - sum);

            cit->order.erase(cit->order.begin(), cit->order.end());
            cit->cart.erase(cit->cart.begin(), cit->cart.end());

            cit->haveOrder = false;

            return true;
        }
    }
    return false;
}

bool Trade::genOrder(const std::string &uname)
{
    for (auto uit : userList)
    {
        auto cit = dynamic_cast<Consumer *>(uit);

        if (uit->getName() == uname && uit->getUserType() == User::consumer && (dynamic_cast<Consumer *>(uit))->haveOrder == false)
        {
            double sum = 0.0;

            for (auto &itemPair : cit->cart)
            {
                if (getQuantity(itemPair.first) < itemPair.second)
                {
                    return false;
                }
            }

            for (auto &itemPair : cit->cart)
            {
                sum += (double)itemPair.second * (double)getPrice(itemPair.first);
                changeQuantity(itemPair.first, adminName, getQuantity(itemPair.first) - itemPair.second);
            }

            (dynamic_cast<Consumer *>(uit))->order = (dynamic_cast<Consumer *>(uit))->cart;
            (dynamic_cast<Consumer *>(uit))->haveOrder = true;
            (dynamic_cast<Consumer *>(uit))->sum = sum;
            std::cout << "Sum : " << sum << std::endl;
            return true;
        }
    }
    return false;
}

bool Trade::delOrder(const std::string &uname)
{
    for (auto &uit : userList)
    {
        if (uit->getName() == uname && uit->getUserType() == User::consumer && (dynamic_cast<Consumer *>(uit))->haveOrder == true)
        {
            for (auto &itemPair : (dynamic_cast<Consumer *>(uit))->order)
            {
                changeQuantity(itemPair.first, adminName, getQuantity(itemPair.first) + itemPair.second);
            }

            auto cit = dynamic_cast<Consumer *>(uit);

            cit->order.erase(cit->order.begin(), cit->order.end());
            cit->haveOrder = false;
            cit->sum = 0;
            return true;
        }
    }
    return false;
}

bool Trade::changeCart(const std::string &uname, const std::string &name, int q)
{
    if (q < 0)
    {
        return false;
    }
    for (auto &uit : userList)
    {
        if (uit->getName() == uname && uit->getUserType() == User::consumer)
        {
            for (auto &itemPair : dynamic_cast<Consumer *>(uit)->cart)
            {
                if (itemPair.first == name)
                {
                    itemPair.second = q;
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}

bool Trade::delCart(const std::string &uname, const std::string &name)
{
    for (auto &uit : userList)
    {
        if (uit->getName() == uname && uit->getUserType() == User::consumer)
        {
            for (auto it = dynamic_cast<Consumer *>(uit)->cart.begin(); it != dynamic_cast<Consumer *>(uit)->cart.end(); it++)
            {
                if (it->first == name)
                {
                    dynamic_cast<Consumer *>(uit)->cart.erase(it);
                    return true;
                }
            }
        }
    }
    return false;
}

int Trade::getQuantity(const std::string &name) const
{
    int ret = -1;
    for (auto it = commList.begin(); it != commList.end(); it++)
    {
        if (name.compare((*it)->getName()) == 0)
        {
            ret = (*it)->getQuantity();
            return ret;
        }
    }
    return ret;
}

double Trade::getPrice(const std::string &name) const
{
    double ret = -1;
    for (auto it = commList.begin(); it != commList.end(); it++)
    {
        if (name.compare((*it)->getName()) == 0)
        {
            ret = (*it)->getPrice();
            return ret;
        }
    }
    return ret;
}

bool Trade::clearCart(const std::string &uname)
{
    for (auto uit : userList)
    {
        if (uname.compare(uit->getName()) == 0 && uit->getUserType() == User::Type::consumer)
        {
            Consumer *it = dynamic_cast<Consumer *>(uit);
            it->cart.erase(it->cart.begin(), it->cart.end());
            return true;
        }
    }
    return false;
}

bool Trade::haveComm(const std::string &name) const
{
    for (auto it : commList)
    {
        if (name.compare(it->getName()) == 0)
        {
            return true;
        }
    }
    return false;
}

bool Trade::addCart(const std::string &uname, const std::string &name, int q)
{
    if (q < 0)
    {
        return false;
    }
    if (haveComm(name) && haveUser(uname))
    {
        for (auto uit : userList)
        {
            if (uname.compare(uit->getName()) == 0 && uit->getUserType() == User::Type::consumer)
            {
                auto it = dynamic_cast<Consumer *>(uit);
                it->addCart(name, q);
                return true;
            }
        }
    }
    return false;
}

bool Trade::redCart(const std::string &uname, const std::string &name, int q)
{
    if (q < 0)
    {
        return false;
    }
    if (haveComm(name) && haveUser(uname))
    {
        for (auto uit : userList)
        {
            if (uname.compare(uit->getName()) == 0 && uit->getUserType() == User::Type::consumer)
            {
                auto it = dynamic_cast<Consumer *>(uit);
                return it->redCart(name, q);
            }
        }
    }
    return false;
}

Trade::Trade(const Trade &t)
{
}

Trade::Trade()
{
    Merchant *upt = new Merchant(adminName, adminPwd, 0);
    userList.push_back(upt);

    commMap.insert(std::pair<std::string, commType>("book", book));
    commMap.insert(std::pair<std::string, commType>("food", food));
    commMap.insert(std::pair<std::string, commType>("cloth", cloth));
}

Trade::~Trade()
{
    for (auto uit : userList)
    {
        delete uit;
    }
}

double Trade::getbal(const std::string &uname) const
{
    for (auto uit : userList)
    {
        if (uname.compare(uit->getName()) == 0)
        {
            return uit->getBalance();
        }
    }
    return -1;
}

bool Trade::setbal(const std::string &uname, double b)
{
    if (b < 0)
    {
        return false;
    }
    for (auto uit : userList)
    {
        if (uname.compare(uit->getName()) == 0)
        {
            uit->setBalance(b);
            return true;
        }
    }
    return false;
}

bool Trade::addbal(const std::string &uname, double b)
{
    if (b < 0)
        return false;
    for (auto uit : userList)
    {
        if (uname.compare(uit->getName()) == 0)
        {
            // uit->setBalance(b + uit->getBalance());
            uit->addBalance(b);
            return true;
        }
    }
    return false;
}

bool Trade::redbal(const std::string &uname, double b)
{
    for (auto uit : userList)
    {
        if (uname.compare(uit->getName()) == 0)
        {
            // uit->setBalance(b - uit->getBalance());
            return uit->redBalance(b);
        }
    }
    return false;
}

bool Trade::setPassword(const std::string &username, const std::string &password)
{
    for (auto uit : userList)
    {
        if (username == uit->getName())
        {
            uit->setPassword(password);
            return true;
        }
    }
    return false;
}

double Trade::getSum(const std::string &uname) const
{
    for (auto uit : userList)
    {
        if (uit->getName() == uname && uit->getUserType() == User::consumer)
        {
            auto cit = dynamic_cast<Consumer *>(uit);
            return cit->sum;
        }
    }
    return -1;
}

bool Trade::refreshOrder()
{
    for (auto uit : userList)
    {
        if (uit->getUserType() == User::Type::consumer)
        {
            auto cit = dynamic_cast<Consumer *>(uit);
            clock_t now;
            now=clock();

            time_t orderTime = cit->getOrderTime();

            if (cit->haveOrder == true && (now - orderTime) / CLOCKS_PER_SEC >= expireTime)
            {
                delOrder(cit->getName());
            }
        }
    }
    return true;
}

int Trade::exec(const std::string &port)
{

    std::cout << "LISTENING : " << port << std::endl;

    int serverFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in myAddr;
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myAddr.sin_port = htons(atoi(port.c_str()));

    bind(serverFd, (sockaddr *)&myAddr, sizeof(myAddr));

    listen(serverFd, 5);

    // char *buffRecv, *buffSend;
    // buffRecv = (char *)malloc(MAXBUF);
    // buffSend = (char *)malloc(MAXBUF);
    char buffRecv[MAXBUF];
    char buffSend[MAXBUF];
    unsigned int len = 0;

    sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);

    int clientFd;

    while (1)
    {
        memset(buffRecv, 0, MAXBUF);
        memset(buffSend, 0, MAXBUF);
        memset(&clientAddr, 0, sizeof(clientAddr));
        len = 0;

        clientFd = accept(serverFd, (sockaddr *)&clientAddr, (socklen_t *)&addrLen);

        std::cout << "Accept a request\n";

        refreshOrder();

        int ret = read(clientFd, buffRecv, MAXBUF);

        std::cout << "len : " << ret << std::endl;

        std::istringstream iss(buffRecv + 2);

        char oper;
        oper = buffRecv[0];

        std::cout << "oper : " << oper << std::endl;

        std::string name, pwd, type, t, cname;
        int token;
        int num;
        double mon;
        std::string desc;

        switch (oper)
        {
        // regis
        case 1:
            iss >> name >> pwd >> type;
            if (!addUser(name, pwd, atoi(type.c_str())))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;

        // login
        case 2:
            iss >> name >> pwd;
            if (!checkPassword(name, pwd))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                token = keyGen(name);
                if (token != -1)
                {
                    buffSend[0] = '1';
                    buffSend[1] = ' ';

                    std::ostrstream oss(buffSend + 2, MAXBUF);

                    // buffSend[2] = token;
                    oss << token;

                    len += MAXBUF;
                }
                else
                {
                    buffSend[0] = '2';
                    len++;
                }
                break;
            }
            break;

        // logout
        case 3:
            iss >> t;
            token = atoi(t.c_str());
            tokenMap.erase(token);
            break;

        // addcart
        case 4:
            iss >> t >> cname >> num;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (!haveComm(cname))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            if (!addCart(name, cname, num))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;

        // delcart
        case 5:
            iss >> t >> cname;
            token = atoi(t.c_str());
            name = tokenMap[token];

            if (!delCart(name, cname))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;

        // chcart
        case 6:
            iss >> t >> cname >> num;
            token = atoi(t.c_str());
            name = tokenMap[token];

            if (!changeCart(name, cname, num))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;

        // clrcart
        case 7:
            iss >> t;
            token = atoi(t.c_str());
            name = tokenMap[token];

            if (!clearCart(name))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }

            break;

        // genorder
        case 8:
            iss >> t;
            token = atoi(t.c_str());
            name = tokenMap[token];

            if (!genOrder(name))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                buffSend[1] = ' ';
                len += 2;
                double d = getSum(name);
                memcpy(buffSend + len, &d, sizeof(d));
                len += sizeof(d);
                break;
            }

            break;

        // delorder
        case 9:
            iss >> t;
            token = atoi(t.c_str());
            name = tokenMap[token];

            if (!delOrder(name))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;

        // settle
        case 10:
            iss >> t;
            token = atoi(t.c_str());
            name = tokenMap[token];

            if (!buy(name))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                buffSend[1] = ' ';
                len += 2;
                double b = getbal(name);
                memcpy(buffSend + len, &b, sizeof(b));
                len += sizeof(b);
                break;
            }
            /* {
                std::string ret = buy(name);
                memcpy(buffSend, ret.c_str(), ret.size());
                len = ret.size();
            } */
            break;

        // recharge
        case 11:
            iss >> t >> mon;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (!addbal(name, mon))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                buffSend[1] = ' ';
                len += 2;
                double b = getbal(name);
                memcpy(buffSend + len, &b, sizeof(b));
                len += sizeof(b);
                break;
            }
            break;

        // ls
        case 12:
            iss >> cname >> type;
            {
                std::string ret = listComm(cname, type);
                memcpy(buffSend, ret.c_str(), ret.size());
                len += ret.size();
            }
            break;

        // lsall
        case 13:
            // use wildcard
            {
                std::string ret = listComm("*", "*");
                memcpy(buffSend, ret.c_str(), ret.size());
                len += ret.size();
            }
            break;

        // lsu
        case 14:
            iss >> name;
            {
                std::string ret = getUserInfo(name);
                memcpy(buffSend, ret.c_str(), ret.size());
                len += ret.size();
            }
            break;

        // addcomm
        case 15:
            iss >> cname >> t >> type >> mon >> desc;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (!addComm(cname, name, type, mon, desc))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;

        // chquantity
        case 16:
            iss >> cname >> t >> num;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (!changeQuantity(cname, name, num))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;

        // chpr
        case 17:
            iss >> cname >> t >> mon;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (!setPrice(cname, name, mon))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;

        // chpercent
        case 18:
            iss >> cname >> t >> mon;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (!setPercent(cname, name, mon))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;

        // chtpercent
        case 19:
            iss >> type >> t >> mon;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (!setPercent(mon, type, name))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;

        // quit
        case 20:
            iss >> t;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (name == adminName)
            {
                buffSend[0] = '1';
                len++;

                saveCommFile();
                saveUserFile();

                return 0;

                break;
            }
            else
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            break;

        // setpw
        case 21:
            iss >> t >> cname;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (!setPassword(name, cname))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;

        // withdraw
        case 22:
            iss >> t >> mon;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (!redbal(name, mon))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                buffSend[1] = ' ';
                len += 2;
                double b = getbal(name);
                memcpy(buffSend + len, &b, sizeof(b));
                len += sizeof(b);
                break;
            }
            break;

        // redcart
        case 23:
            iss >> t >> cname >> num;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (!haveComm(cname))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            if (!redCart(name, cname, num))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;
        }
        send(clientFd, buffSend, len, 0);
        close(clientFd);

        refreshOrder();
        saveCommFile(true);
        saveUserFile(true);
    }
}

int Trade::keyGen(const std::string &name)
{
    int ret = -1;

    for (auto it : tokenMap)
    {
        if (it.second == name)
        {
            return it.first;
        }
    }

    if (tokenMap.size() == MAXMAP)
    {
        return ret;
    }
    for (int i = 0; i < MAXMAP; i++)
    {
        auto it = tokenMap.find(i);
        if (it == tokenMap.end())
        {
            ret = i;
            tokenMap[i] = name;
            break;
        }
    }
    return ret;
}
