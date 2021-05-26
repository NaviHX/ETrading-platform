#ifndef __COMMODITY_H__
#define __COMMODITY_H__

/*
 * Copyright © 2021 Navi.HuangXing. All rights reserved.
 * @author      : Navi.HuangXing 
 * @date        : 2021-04-24
 * @file        : Commidity.h
 * @description : 商品模块
*/

#include <string>

class Commdity
{
private:
    double price;
    double percent;
    int quantity;
    std::string comType;
    std::string name;

    std::string owner;
    std::string description;

public:
    virtual double getPrice() const { return price * percent; } // 获得计算折扣后的价格
    double getOriginPrice() const { return price; }             // 获得原价
    void setOriginPrice(double p) { price = p; }                // 设置原价

    int getQuantity() const { return quantity; } // 获得数量
    void setQuantity(int q) { quantity = q; }    // 设置数量

    double getPercent() const { return percent; }                   // 获得折扣
    void setPercent(double p) { percent = p; }                      // 设置折扣
    virtual double getRealDiscount() const { return getPercent(); } // 综合计算品种折扣+该商品折扣

    std::string getComType() const { return comType; }     // 获得商品类型
    void setComType(const std::string &t) { comType = t; } // 设置商品类型

    std::string getName() const { return name; }     // 获得商品名称
    void setName(const std::string &n) { name = n; } // 设置商品名称

    std::string getOwner() const { return owner; }      // 获得所有人
    std::string getDesc() const { return description; } // 设置所有人

    Commdity(double pr, double pe, int q, const std::string &ct, const std::string &n, const std::string &own, const std::string &desc);

    bool buy(int count);               // 购买商品
    bool isType(const std::string &t); // 判断商品类型
};

class Food : public Commdity
{
protected:
    static double discount; // 该种类商品共享的折扣

public:
    virtual double getPrice() const { return getOriginPrice() * getPercent() * discount; }
    static void setDiscount(double d) { discount = d; }
    static double getDiscount() { return discount; }
    virtual double getRealDiscount() const { return discount * getPercent(); }

    Food(double pr, double pe, int q, const std::string &ct, const std::string &n, const std::string &own, const std::string &desc);
};

class Book : public Commdity
{
protected:
    static double discount; // 该种类商品共享的折扣

public:
    virtual double getPrice() const { return getOriginPrice() * getPercent() * discount; }
    static void setDiscount(double d) { discount = d; }
    static double getDiscount() { return discount; }
    virtual double getRealDiscount() const { return discount * getPercent(); }

    Book(double pr, double pe, int q, const std::string &ct, const std::string &n, const std::string &own, const std::string &desc);
};

class Cloth : public Commdity
{
protected:
    static double discount; // 该种类商品共享的折扣

public:
    virtual double getPrice() const { return getOriginPrice() * getPercent() * discount; }
    static void setDiscount(double d) { discount = d; }
    static double getDiscount() { return discount; }
    virtual double getRealDiscount() const { return discount * getPercent(); }

    Cloth(double pr, double pe, int q, const std::string &ct, const std::string &n, const std::string &own, const std::string &desc);
};

#endif
