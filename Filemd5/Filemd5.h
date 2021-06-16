#ifndef __FILEMD5_H__
#define __FILEMD5_H__

/*
 * Copyright © 2021 Navi.HuangXing. All rights reserved.
 * @author      : Navi.HuangXing 
 * @date        : 2021-06-16
 * @file        : Filemd5.h
 * @description : 计算文件md5校验和，校验文件
*/

#include <string>

class Filemd5
{
public:
    static std::string getMd5Sum(const std::string &fp);
    static void writeMd5Sum(const std::string& fp,const std::string& sumfp);
    static bool verifyMd5Sum(const std::string &fp, const std::string &sumfp);

    static const unsigned int MAXBUFFLENGTH = 1024;
    static const unsigned int MD5LENGTH = 16;
};

#endif
