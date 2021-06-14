#include "Application.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <sstream>
#include <strstream>

Application::Application(const std::string &un, const std::string &pwd) : uname(un), password(pwd), logged(false)
{
    logged = false;
}

int Application::exec(const std::string &ip, const std::string &port)
{
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(port.c_str()));
    inet_aton(ip.c_str(), &serverAddr.sin_addr);

    std::cout << "#===================================#\n"
              << " ETrading-Platform V3 Client\n"
              << " Build : "__DATE__" "__TIME__"\n"
              << "#===================================#\n"
              << "Type \'help\' for information\n";

    std::map<std::string, strValue> strMap;

    strMap["help"] = strValue::help;
    strMap["regis"] = strValue::regis;
    strMap["login"] = strValue::login;
    strMap["logout"] = strValue::logout;
    strMap["addcart"] = strValue::addcart;
    strMap["delcart"] = strValue::delcart;
    strMap["chcart"] = strValue::chcart;
    strMap["clrcart"] = strValue::clrcart;
    strMap["genorder"] = strValue::genorder;
    strMap["delorder"] = strValue::delorder;
    strMap["settle"] = strValue::settle;
    strMap["recharge"] = strValue::recharge;
    strMap["ls"] = strValue::ls;
    strMap["lsall"] = strValue::lsall;
    strMap["lsu"] = strValue::lsu;
    strMap["addcomm"] = strValue::addcomm;
    strMap["chquantity"] = strValue::chquantity;
    strMap["chpr"] = strValue::chpr;
    strMap["chpercent"] = strValue::chpercent;
    strMap["chtpercent"] = strValue::chtpercent;
    strMap["quit"] = strValue::quit;
    strMap["setpw"] = strValue::setpw;
    strMap["withdraw"] = strValue::withdraw;
    strMap["redcart"] = strValue::redcart;

    char buffRecv[MAXBUF];
    char buffSend[MAXBUF];

    std::string oper;
    std::cout << "> ";
    // while (std::cin >> oper)
    while (std::getline(std::cin, oper))
    {
        int clientFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        memset(buffRecv, 0, MAXBUF);
        memset(buffSend, 0, MAXBUF);

        int len;

        auto argv = splitStr(oper);
        if (argv.size() > 0)
        {
            auto it = strMap.find(argv[0]);
            if (it != strMap.end())
            {
                switch (strMap[argv[0]])
                {
                case help:
                    std::cout << "help : help info\n"
                              << "regis <name> <password> <0/1> : register a user\n"
                              << "login <name> <password> : login as a user\n"
                              << "logout : logout\n"
                              << "setpw <new password> : set password\n"
                              << "addcart <commdity name> <number> : add a commdity into cart\n"
                              << "redcart <commdity name> <number> : reduce quantity\n"
                              << "delcart <commdity name> : delete a item\n"
                              << "chcart <commdity name> <number> : change quantity in cart\n"
                              << "clrcart : clear cart\n"
                              << "genorder : generate the order\n"
                              << "delorder : delete the order\n"
                              << "settle : settle the order\n"
                              << "recharge <number> : recharge\n"
                              << "withdraw <number> : withdraw\n"
                              << "ls <commdity name> <commdity type> : list commdity\n"
                              << "lsall : list all commdity\n"
                              << "lsu <username> : list a user info\n"
                              << "addcomm <commdity name> <commdity type> <price> <description>: add a commdity\n"
                              << "chquantity <commdity name> <number> : change quantity\n"
                              << "chpr <commdity name> <number> : change price\n"
                              << "chpercent <commdity name> <number> : change discount\n"
                              << "chtpercent <type> <number>\n"
                              << "quit : quit\n";
                    break;

                case addcart:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }
                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    // 类型转换 str->数字
                    std::istringstream iss(argv[2]);
                    int q;
                    if (!(iss >> q))
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    if (iss >> oper)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    // 连接服务器
                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    // 构造报文
                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(addcart)
                        << " " << token
                        << " " << argv[1]
                        << " " << q;

                    // 发送并接收响应
                    write(clientFd, buffSend, MAXBUF);
                    recv(clientFd, buffRecv, MAXBUF, 0);

                    std::istringstream recs(buffRecv);

                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                        break;
                    }
                    break;
                }

                case settle:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(settle)
                        << " " << token;

                    write(clientFd, buffSend, MAXBUF);
                    recv(clientFd, buffRecv, MAXBUF, 0);

                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                        break;
                    }
                    else
                    {
                        std::cout << "Balance : " << *(double *)(buffRecv + 2) << std::endl;
                        break;
                    }
                    break;
                }

                case delorder:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(delorder)
                        << " " << token;

                    write(clientFd, buffSend, MAXBUF);
                    recv(clientFd, buffRecv, MAXBUF, 0);

                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                    }
                    break;
                }

                case genorder:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(genorder)
                        << " " << token;

                    write(clientFd, buffSend, MAXBUF);
                    recv(clientFd, buffRecv, MAXBUF, 0);

                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                    }
                    else
                    {
                        std::cout << "Sum : " << *(double *)(buffRecv + 2) << std::endl;
                        unsigned i = 2 + sizeof(double);
                        while (buffRecv[i] != '\0')
                        {
                            std::cout << buffRecv[i];
                            i++;
                        }
                    }
                    break;
                }

                case clrcart:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(clrcart)
                        << " " << token;

                    write(clientFd, buffSend, MAXBUF);
                    recv(clientFd, buffRecv, MAXBUF, 0);

                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                    }
                    break;
                }

                case delcart:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }
                    if (argv.size() < 2)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(delcart)
                        << " " << token
                        << " " << argv[1];

                    write(clientFd, buffSend, MAXBUF);
                    recv(clientFd, buffRecv, MAXBUF, 0);

                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                    }
                    break;
                }

                case chcart:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }
                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    std::istringstream iss(argv[2]);
                    int q;
                    if (!(iss >> q))
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    if (iss >> oper)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(chcart)
                        << " " << token
                        << " " << argv[1]
                        << " " << q;

                    write(clientFd, buffSend, MAXBUF);
                    recv(clientFd, buffRecv, MAXBUF, 0);

                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                        break;
                    }
                    break;
                }

                case regis:
                {
                    if (argv.size() < 4)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    std::istringstream iss(argv[3]);
                    int t;
                    if (!(iss >> t))
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    if (iss >> oper)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(regis)
                        << " " << argv[1]
                        << " " << argv[2]
                        << " " << t;

                    write(clientFd, buffSend, MAXBUF);
                    recv(clientFd, buffRecv, MAXBUF, 0);

                    if (buffRecv[0] == '1')
                    {
                        std::cout << "User [ " << argv[1] << " ] added\n"
                                  << "Please login again\n";
                    }
                    else
                    {
                        std::cout << "Failed\n";
                    }
                }
                break;

                case login:
                {
                    if (isLogged())
                    {
                        std::cout << "Already logged\n";
                        break;
                    }
                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(login)
                        << " " << argv[1]
                        << " " << argv[2];

                    write(clientFd, buffSend, MAXBUF);
                    recv(clientFd, buffRecv, MAXBUF, 0);

                    if (buffRecv[0] == '1')
                    {
                        uname = argv[1];
                        password = argv[2];
                        logged = true;
                        std::cout << "Logged in\n";
                        std::istringstream iss(buffRecv + 2);
                        iss >> token;
                    }
                    else
                    {
                        std::cout << "Failed\n";
                    }
                }
                break;

                case ls:
                {
                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(ls)
                        << " " << argv[1]
                        << " " << argv[2];

                    write(clientFd, buffSend, MAXBUF);
                    len = recv(clientFd, buffRecv, MAXBUF, 0);

                    for (int i = 0; i < len; i++)
                    {
                        std::cout << buffRecv[i];
                    }
                }
                break;

                case lsall:
                {
                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(lsall)
                        << " " << token;

                    write(clientFd, buffSend, MAXBUF);
                    len = recv(clientFd, buffRecv, MAXBUF, 0);

                    for (int i = 0; i < len; i++)
                    {
                        std::cout << buffRecv[i];
                    }
                }

                break;

                case addcomm:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }
                    if (argv.size() < 4)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    std::istringstream iss(argv[3]);
                    double p;
                    if (!(iss >> p))
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    if (iss >> oper)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(addcomm)
                        << " " << argv[1]
                        << " " << token
                        << " " << argv[2]
                        << " " << p
                        << " " << argv[4];

                    write(clientFd, buffSend, MAXBUF);
                    recv(clientFd, buffRecv, MAXBUF, 0);

                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                    }
                }
                break;

                case chquantity:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }
                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    std::istringstream iss(argv[2]);
                    int q;
                    if (!(iss >> q))
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    if (iss >> oper)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(chquantity)
                        << " " << argv[1]
                        << " " << token
                        << " " << q;

                    write(clientFd, buffSend, MAXBUF);
                    recv(clientFd, buffRecv, MAXBUF, 0);

                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                    }
                }
                break;

                case chpr:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }
                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    std::istringstream iss(argv[2]);
                    double p;
                    if (!(iss >> p))
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    if (iss >> oper)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(chpr)
                        << " " << argv[1]
                        << " " << token
                        << " " << p;

                    write(clientFd, buffSend, MAXBUF);
                    recv(clientFd, buffRecv, MAXBUF, 0);

                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                    }
                }
                break;

                case chpercent:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }
                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    std::istringstream iss(argv[2]);
                    double p;
                    if (!(iss >> p))
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    if (iss >> oper)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(chpercent)
                        << " " << argv[1]
                        << " " << token
                        << " " << p;

                    write(clientFd, buffSend, MAXBUF);

                    recv(clientFd, buffRecv, MAXBUF, 0);
                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                    }
                }
                break;

                case chtpercent:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }

                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    std::istringstream iss(argv[2]);
                    double p;
                    if (!(iss >> p))
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    if (iss >> oper)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(chtpercent)
                        << " " << argv[1]
                        << " " << token
                        << " " << p;

                    write(clientFd, buffSend, MAXBUF);
                    recv(clientFd, buffRecv, MAXBUF, 0);

                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                    }
                }
                break;

                case lsu:
                {
                    if (argv.size() < 2)
                    {
                        std::cout << "INVALID format\n";
                        break;
                    }
                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(lsu)
                        << " " << argv[1];

                    write(clientFd, buffSend, MAXBUF);
                    len = recv(clientFd, buffRecv, MAXBUF, 0);

                    for (int i = 0; i < len; i++)
                    {
                        std::cout << buffRecv[i];
                    }
                }
                break;

                case quit:
                {
                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    if (isLogged())
                    {
                        // std::string buff;
                        std::ostrstream oss(buffSend, MAXBUF);
                        oss << static_cast<char>(quit)
                            << " " << token;

                        write(clientFd, buffSend, MAXBUF);
                        recv(clientFd, buffRecv, MAXBUF, 0);
                    }

                    std::cout << "Quit.\n";
                    close(clientFd);
                    return 0;
                }

                case setpw:
                {
                    if (argv.size() < 2)
                    {
                        std::cout << "INVALID format\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(setpw)
                        << " " << token
                        << " " << argv[1];
                    write(clientFd, buffSend, MAXBUF);
                    recv(clientFd, buffRecv, MAXBUF, 0);
                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                    }
                }
                break;

                case recharge:
                    if (argv.size() < 2)
                    {
                        std::cout << "Failed\n";
                        break;
                    }
                    else
                    {
                        std::istringstream iss(argv[1]);
                        double b;
                        if (!(iss >> b))
                        {
                            std::cout << "INVALID Format\n";
                            break;
                        }
                        if (iss >> oper)
                        {
                            std::cout << "INVALID Format\n";
                            break;
                        }

                        if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                        {
                            std::cout << "CANNOT connect server\n";
                            return 0;
                        }

                        std::ostrstream oss(buffSend, MAXBUF);
                        oss << static_cast<char>(recharge)
                            << " " << token
                            << " " << b;
                        write(clientFd, buffSend, MAXBUF);
                        recv(clientFd, buffRecv, MAXBUF, 0);
                        if (buffRecv[0] == '0')
                        {
                            std::cout << "Failed\n";
                        }
                    }
                    break;

                case withdraw:
                    if (argv.size() < 2)
                    {
                        std::cout << "Failed\n";
                        break;
                    }
                    else
                    {
                        std::istringstream iss(argv[1]);
                        double b;
                        if (!(iss >> b))
                        {
                            std::cout << "INVALID Format\n";
                            break;
                        }
                        if (iss >> oper)
                        {
                            std::cout << "INVALID Format\n";
                            break;
                        }

                        if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                        {
                            std::cout << "CANNOT connect server\n";
                            return 0;
                        }

                        std::ostrstream oss(buffSend, MAXBUF);
                        oss << static_cast<char>(withdraw)
                            << " " << token
                            << " " << b;
                        write(clientFd, buffSend, MAXBUF);
                        recv(clientFd, buffRecv, MAXBUF, 0);
                        if (buffRecv[0] == '0')
                        {
                            std::cout << "Failed\n";
                            break;
                        }
                        std::cout << "Withdraw : " << b << std::endl;
                    }
                    break;

                case logout:
                {
                    logged = false;

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    // std::string buff;
                    std::ostrstream oss(buffSend, MAXBUF);

                    oss << static_cast<char>(logout)
                        << " " << token;

                    write(clientFd, buffSend, MAXBUF);
                    recv(clientFd, buffRecv, MAXBUF, 0);
                }
                break;

                case redcart:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }
                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    // 类型转换 str->数字
                    std::istringstream iss(argv[2]);
                    int q;
                    if (!(iss >> q))
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    if (iss >> oper)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    // 连接服务器
                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    // 构造报文
                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(redcart)
                        << " " << token
                        << " " << argv[1]
                        << " " << q;

                    // 发送并接收响应
                    write(clientFd, buffSend, MAXBUF);
                    recv(clientFd, buffRecv, MAXBUF, 0);

                    std::istringstream recs(buffRecv);

                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                        break;
                    }
                    break;
                }

                default:
                    std::cout << "ILLEGAL arg : " << argv[0] << " . Type help for more info" << std::endl;
                    break;
                }
            }
            else
            {
                std::cout << "ILLEGAL arg : " << argv[0] << " . Type help for more info" << std::endl;
            }
        }
        close(clientFd);
        if (isLogged())
            std::cout << uname << "@" << ip << ":" << port << " ";
        std::cout << "> ";
    }
    return 0;
}

std::vector<std::string> Application::splitStr(const std::string &str, const char split)
{
    std::vector<std::string> ret;
    std::istringstream iss(str);
    std::string buf;
    while (std::getline(iss, buf, split))
    {
        ret.push_back(buf);
    }
    return ret;
}
