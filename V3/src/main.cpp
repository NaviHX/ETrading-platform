#include "Application.h"
#include "Trade.h"

#include <iostream>
#include <string>
#include <cstring>

int main(int argc, char **argv)
{

    if (argc <= 1)
    {
        std::cout << "Usage :\n"
                  << "Start a server : app -s <port>\n"
                  << "Run a client   : app -c <ip> <port>\n"
                  << "Help info      : app -h\n";
    }
    else
    {
        if (strcmp(argv[1], "-s") == 0)
        {
            if (argc < 3)
                std::cout << "Expected : <port>\n";
            else
            {
                Trade trade;
                trade.readUserFile();
                trade.readCommFile();
                return trade.exec(argv[2]);
            }
        }
        else if (strcmp(argv[1], "-c") == 0)
        {
            if (argc < 4)
                std::cout << "Expected : <ip> <port>\n";
            else
            {
                Application app("", "");
                return app.exec(argv[2], argv[3]);
            }
        }
        else
        {
            std::cout << "Usage :\n"
                      << "Start a server : app -s <port>\n"
                      << "Run a client   : app -c <ip> <port>\n"
                      << "Help info      : app -h\n";
        }
    }
    return 0;
}
