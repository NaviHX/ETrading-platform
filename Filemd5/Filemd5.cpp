#include "Filemd5.h"
#include <openssl/md5.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>

std::string Filemd5::getMd5Sum(const std::string &fp)
{
    std::ifstream ifile(fp, std::ios::in | std::ios::binary);

    unsigned char result[MD5LENGTH];

    if (ifile.fail())
    {
        std::cout << "No such file :" << fp << std::endl;
        return std::string("");
    }

    MD5_CTX ctx;
    MD5_Init(&ctx);

    char databuf[MAXBUFFLENGTH];
    memset(databuf, 0, MAXBUFFLENGTH);

    while (!ifile.eof())
    {
        ifile.read(databuf, MAXBUFFLENGTH);
        int length = ifile.gcount();
        if (length)
        {
            MD5_Update(&ctx, databuf, MAXBUFFLENGTH);
        }
    }

    MD5_Final(result, &ctx);

    std::ostringstream oss;
    for (unsigned int i = 0; i < MD5LENGTH; i++)
    {
        oss << std::hex << (int)result[i];
    }

    ifile.close();
    return oss.str();
}

void Filemd5::writeMd5Sum(const std::string &fp, const std::string &sumfp)
{
    std::string result = getMd5Sum(fp);
    std::ofstream ofile(sumfp, std::ios::out);
    ofile << result << std::endl;
    ofile.close();
}

bool Filemd5::verifyMd5Sum(const std::string &fp, const std::string &sumfp)
{
    std::string result = getMd5Sum(fp);
    std::ifstream ifile(sumfp, std::ios::in);

    if (ifile.fail())
    {
        return false;
    }

    std::string sum;
    ifile >> sum;
    ifile.close();
    return sum == result;
}
