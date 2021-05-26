#ifndef __APPLICATION_H__
#define __APPLICATION_H__

/*
 * Copyright © 2021 Navi.HuangXing. All rights reserved.
 * @author      : Navi.HuangXing 
 * @date        : 2021-05-01
 * @file        : Application.h
 * @description : 客户端
*/

#include <string>

#include "Trade.h"

class Application
{
private:
    std::string uname; // 用户名
    std::string password; // 密码
    // int token; // 远程服务器使用，防止每个报文中都有用户名与密码

    Trade *trade; // 购物平台
    bool logged; // 是否登录

public:
    enum strValue
    {
        help,
        regis,
        login,
        // logout,
        // addcart,
        // settle,
        recharge,
        ls,
        lsall,
        lsu,
        addcomm,
        chquantity,
        chpr,
        chpercent,
        chtpercent,
        quit,
        setpw,
        withdraw
    };

    bool isLogged() { return logged; }
    // bool logout(); // 远程时使用，释放token

    std::string getUname() { return uname; }
    void setUname(const std::string &un) { uname = un; }
    std::string getPassword() { return password; }
    void setPassword(const std::string &pwd) { password = pwd; }

    int exec(); // 进入事件循环

    static std::vector<std::string> splitStr(const std::string &str, const char split = ' '); // 分割命令

    Application(const std::string &un, const std::string &pwd);
};

#endif
