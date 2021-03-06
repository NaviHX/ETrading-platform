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
#include <map>

#include "User.h"
#include "Commidity.h"

const std::string adminName = "admin";
const std::string adminPwd = "123456";

const clock_t expireTime = 60;

class Trade
{
private:
    std::vector<User *> userList;
    std::vector<Commdity *> commList;
    Trade(const Trade &t);

    enum commType
    {
        book,
        food,
        cloth
    };

    std::map<std::string, commType> commMap;

public:
    bool readUserFile(bool quiet = false, const std::string &fp = "userdata.txt", const std::string &sumfp = "usermd5.txt");
    bool saveUserFile(bool quiet = false, const std::string &fp = "userdata.txt", const std::string &sumfp = "usermd5.txt") const;
    bool readCommFile(bool quiet = false, const std::string &fp = "commdata.txt", const std::string &sumfp = "commmd5.txt");
    bool saveCommFile(bool quiet = false, const std::string &fp = "commdata.txt", const std::string &sumfp = "commmd5.txt") const;

    // 用户管理
    bool haveUser(const std::string &username) const;
    bool checkPassword(const std::string &username, const std::string &password) const;
    bool addUser(const std::string &username, const std::string &password, int type);
    bool delUser(const std::string &username);
    bool getUserInfo(const std::string &username) const;
    double getbal(const std::string &uname) const;
    bool setbal(const std::string &uname, double b);
    bool addbal(const std::string &uname, double b);
    bool redbal(const std::string &uname, double b);
    bool setPassword(const std::string &username, const std::string &password);

    bool refreshOrder(); // 根据时间刷新订单列表，删除超时订单

    // 商品管理
    bool haveComm(const std::string &name) const;
    void listComm() const;
    void listComm(const std::string &name, const std::string &comType = "*", const std::string &uname = "") const;
    bool addComm(const std::string &name, const std::string &uname, const std::string &comType, double price, const std::string &desc);
    bool delComm(const std::string &name, const std::string &uname);
    bool changeQuantity(const std::string &name, const std::string &uname, int q);
    int getQuantity(const std::string &name) const;
    bool setPrice(const std::string &name, const std::string &uname, double p);
    double getPrice(const std::string &name) const;
    bool setPercent(const std::string &name, const std::string &uname, double p);
    bool setPercent(double p, const std::string &type, const std::string &uname);
    std::string getOwner(const std::string &name);

    // 购物
    bool buy(const std::string &uname);
    bool genOrder(const std::string &uname);
    bool delOrder(const std::string &uname);
    bool clearCart(const std::string &uname);
    bool listCart(const std::string &uname);
    bool addCart(const std::string &uname, const std::string &name, int q);
    bool redCart(const std::string &uname, const std::string &name, int q);
    bool changeCart(const std::string &uname, const std::string &name, int q);
    bool delCart(const std::string &uname, const std::string &name);

    ~Trade();
    Trade();
};

#endif
