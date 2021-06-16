#include "Filemd5.h"
#include <iostream>

int main()
{
    std::string result = Filemd5::getMd5Sum("test.txt");
    std::cout << "File md5sum :" << result << std::endl;

    Filemd5::writeMd5Sum("test.txt", "md5sum.txt");

    if (Filemd5::verifyMd5Sum("test.txt", "md5sum.txt"))
    {
        std::cout << "Verify successfullt\n";
    }

    return 0;
}
