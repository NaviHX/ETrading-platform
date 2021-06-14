#ifndef __USER_H__
#define __USER_H__

/*
 * Copyright © 2021 Navi.HuangXing. All rights reserved.
 * @author      : Navi.HuangXing 
 * @date        : 2021-04-24
 * @file        : User.h
 * @description : 用户类
*/

#include <string>
#include <vector>
#include <utility>

class User
{
private:
    std::string name;
    std::string password;
    double balance;

public:
    enum Type
    {
        consumer,
        merchant,
    };
    virtual Type getUserType() = 0; // 获得用户类型

    double getBalance() const { return balance; } // 获得余额
    void setBalance(double b) { balance = b; }    // 设置余额
    void addBalance(double b) { balance += b; }   // 增加余额
    bool redBalance(double b)                     // 减少余额
    {
        if (balance > b)
        {
            balance -= b;
            return true;
        }
        return false;
    }

    std::string getName() const { return name; }               // 获得用户名
    void setName(const std::string &nm) { name = nm; }         // 设置用户名
    std::string getPassword() const { return password; }       // 获得密码
    void setPassword(const std::string &pw) { password = pw; } // 设置密码

    User(const std::string &n, const std::string &pw, const double b); // 构造函数
    virtual ~User() {}
};

class Consumer : public User
{
private:
    clock_t orderTime;

public:
    std::vector<std::pair<std::string, int>> cart;
    std::vector<std::pair<std::string, int>> order;

    bool haveOrder = false;
    double sum = 0;

    virtual Type getUserType() override { return Type::consumer; }
    std::vector<std::pair<std::string, int>> getCart() const { return cart; }
    bool addCart(std::string comId, int num); // 加入购物车
    bool redCart(std::string comId, int num); // 减少购物车

    clock_t getOrderTime() const { return orderTime; }
    void setOrderTime() { orderTime = clock(); }

    Consumer(const std::string &n, const std::string &pw, const double b);
    virtual ~Consumer() {}
};

class Merchant : public User
{
private:
public:
    virtual Type getUserType() override { return Type::merchant; }

    Merchant(const std::string &n, const std::string &pw, const double b);
    virtual ~Merchant() {}
};

#endif
