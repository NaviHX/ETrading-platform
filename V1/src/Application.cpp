#include "Application.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

Application::Application(const std::string &un, const std::string &pwd) : uname(un), password(pwd), logged(false)
{
    trade = new Trade;
    trade->readCommFile();
    trade->readUserFile();
    logged = false;
}

int Application::exec()
{
    std::cout << "ETrading-Platform V1\n"
              << "help : help info\n"
              << "regis <name> <password> <0/1> : register a user\n"
              << "login <name> <password> : login as a user\n"
              << "logout : logout\n"
              << "addcart <commdity name> <number> : add a commdity into cart\n"
              << "delcart <commdity name> : delete a item\n"
              << "chcart <commdity name> <number> : change quantity in cart\n"
              << "clrcart : clear cart\n"
              << "genorder : generate the order\n"
              << "delorder : delete the order\n"
              << "settle : settle the order\n"
              << "recharge <number> : recharge\n"
              << "ls <commdity name> <commdity type> : list commdity\n"
              << "lsall : list all commdity\n"
              << "lsu <username> : list a user info\n"
              << "addcomm <commdity name> <commdity type> <price> <description>: add a commdity\n"
              << "chquantity <commdity name> <number> : change quantity\n"
              << "chpr <commdity name> <number> : change price\n"
              << "chpercent <commdity name> <number> : change discount\n"
              << "chtpercent <type> <number>\n"
              << "help : help info\n"
              << "quit : quit\n";

    std::map<std::string, strValue> strMap;

    strMap["help"] = strValue::help;
    strMap["regis"] = strValue::regis;
    strMap["login"] = strValue::login;
    // strMap["logout"]=strValue::logout;
    // strMap["addcart"]=strValue::addcart;
    // strMap["settle"]=strValue::settle;
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

    std::string oper;
    std::cout << "> ";
    // while (std::cin >> oper)
    while (std::getline(std::cin, oper))
    {
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
                              //   << "addcart <commdity name> : add a commdity into cart\n"
                              //   << "settle : settle\n"
                              //   << "recharge <number> : recharge\n"
                              << "ls <commdity name> [commdity type] : list commdity\n"
                              << "lsall : list all commdity\n"
                              << "lsu <username> : list a user info\n"
                              << "addcomm <commdity name> <commdity type> <price> <description>: add a commdity\n"
                              << "chquantity <commdity name> <number> : change quantity\n"
                              << "chpr <commdity name> <number> : change price\n"
                              << "chpercent <commdity name> <number> : change discount\n"
                              << "chtpercent <type> <number>\n"
                              << "withdraw <number> : withdraw\n"
                              << "help\n"
                              << "quit : quit\n";
                    break;

                case regis:
                {
                    if (argv.size() < 4)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    std::istringstream iss(argv[3]);
                    int t;
                    iss >> t;
                    if (trade->addUser(argv[1], argv[2], t))
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
                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    if (trade->checkPassword(argv[1], argv[2]))
                    {
                        uname = argv[1];
                        password = argv[2];
                        logged = true;
                        std::cout << "Logged in\n";
                    }
                    else
                    {
                        std::cout << "Failed\n";
                    }
                    break;

                case ls:
                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    trade->listComm(argv[1], argv[2]);
                    break;

                case lsall:
                    trade->listComm("*", "*");
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
                    iss >> p;
                    if (!trade->addComm(argv[1], uname, argv[2], p, argv[4]))
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
                    iss >> q;
                    if (!trade->changeQuantity(argv[1], uname, q))
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
                    iss >> p;
                    if (!trade->setPrice(argv[1], uname, p))
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
                    iss >> p;
                    if (!trade->setPercent(argv[1], uname, p))
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
                    iss >> p;
                    if (!trade->setPercent(p, argv[1], uname))
                    {
                        std::cout << "Failed\n";
                    }
                }
                break;

                case lsu:
                    if (argv.size() < 2)
                    {
                        std::cout << "INVALID format\n";
                        break;
                    }
                    if (!trade->getUserInfo(argv[1]))
                    {
                        std::cout << "Failed\n";
                    }
                    break;

                case quit:
                    trade->saveCommFile();
                    trade->saveUserFile();
                    std::cout << "Quit.\n";
                    return 0;
                    break;

                case setpw:
                    if (argv.size() < 2)
                    {
                        std::cout << "INVALID format\n";
                        break;
                    }
                    if (!trade->setPassword(uname, argv[1]))
                    {
                        std::cout << "Failed\n";
                    }
                    break;

                case recharge:
                    if (argv.size() < 2)
                    {
                        std::cout << "INVALID format\n";
                        break;
                    }
                    else
                    {
                        std::istringstream iss(argv[1]);
                        double b;
                        iss >> b;
                        if (!trade->addbal(uname, b))
                        {
                            std::cout << "Failed\n";
                        }
                    }
                    break;

                case withdraw:
                    if (argv.size() < 2)
                    {
                        std::cout << "INVALID format\n";
                        break;
                    }
                    else
                    {
                        std::istringstream iss(argv[1]);
                        double b;
                        iss >> b;
                        if (!trade->redbal(uname, b))
                        {
                            std::cout << "Failed\n";
                            break;
                        }
                        std::cout << "Withdraw : " << b << std::endl;
                    }
                    break;

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
        trade->saveCommFile(true);
        trade->saveUserFile(true);

        if (isLogged())
            std::cout << uname << " ";

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
