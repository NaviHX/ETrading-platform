#ifndef __TRADE_H__
#define __TRADE_H__

/*
 * Copyright © 2021 Navi.HuangXing. All rights reserved.
 * @author      : Navi.HuangXing 
 * @date        : 2021-04-24
 * @file        : Trade.h
 * @description : 交易核心系统模块
 *                控制商品信息，用户信息，控制用户权限
 *                为消费者提供结算购物车功能
 *                为商户提供管理商品的功能
*/

#include <string>
#include <vector>
#include <utility>

#include "User.h"
#include "Commidity.h"

const std::string adminName = "admin";
const std::string adminPwd = "123456";

class Trade
{
private:
    std::vector<User *> userList;
    std::vector<Commdity> commList;
    Trade(const Trade &t);

public:
    bool readUserFile(const std::string &fp = "userdata.txt");
    bool saveUserFile(const std::string &fp = "userdata.txt") const;
    bool readCommFile(const std::string &fp = "commdata.txt");
    bool saveCommFile(const std::string &fp = "commdata.txt") const;

    // 用户管理
    bool haveUser(const std::string &username) const;
    bool checkPassword(const std::string &username, const std::string &password) const;
    bool addUser(const std::string &username, const std::string &password, int type);
    bool delUser(const std::string &username);
    bool getUserInfo(const std::string &username) const;
    double getbal(const std::string &uname) const;
    bool setbal(const std::string &uname, double b);
    bool addbal(const std::string &uname, double b);

    // 商品管理
    bool haveComm(const std::string &name) const;
    void listComm() const;
    void listComm(const std::string &name, const std::string &comType = "*", const std::string &uname = "") const;
    bool addComm(const std::string &name, const std::string &uname, const std::string &comType, double price);
    bool delComm(const std::string &name, const std::string &uname);
    bool changeQuantity(const std::string &name, const std::string &uname, int q);
    int getQuantity(const std::string &name) const;
    bool setPrice(const std::string &name, const std::string &uname, double p);
    double getPrice(const std::string &name) const;
    bool setPercent(const std::string &name, const std::string &uname, double p);
    bool setPercent(double p, const std::string &type, const std::string &uname);

    // 购物
    bool buy(const std::string &uname);
    bool clearCart(const std::string &uname);
    bool addCart(const std::string &uname, const std::string &name, int q);

    ~Trade();
    Trade();
};

#endif
