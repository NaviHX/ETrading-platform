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

const int MAXBUF = 2048;

class Application
{
private:
    std::string uname;
    std::string password;
    char token; // 远程服务器使用，防止每个报文中都有用户名与密码

    bool logged;

public:
    enum strValue
    {
        help,
        regis,
        login,
        logout,
        addcart,
        settle,
        recharge,
        ls,
        lsall,
        lsu,
        addcomm,
        chquantity,
        chpr,
        chpercent,
        chtpercent,
        quit
    };

    bool isLogged() { return logged; }
    // bool logout(); // 远程时使用，释放token

    std::string getUname() { return uname; }
    void setUname(const std::string &un) { uname = un; }
    std::string getPassword() { return password; }
    void setPassword(const std::string &pwd) { password = pwd; }

    int exec(const std::string &ip, const std::string &port);

    static std::vector<std::string> splitStr(const std::string &str, const char split = ' ');

    Application(const std::string &un, const std::string &pwd);
};

#endif
