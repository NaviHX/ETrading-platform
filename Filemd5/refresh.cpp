#include "Filemd5.h"
#include <iostream>

int main()
{
    Filemd5::writeMd5Sum("userdata.txt", "usermd5.txt");
    Filemd5::writeMd5Sum("commdata.txt", "commmd5.txt");

    return 0;
}
