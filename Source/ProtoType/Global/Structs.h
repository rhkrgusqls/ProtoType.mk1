#pragma once
#include <iostream>
#include <string.h>
#include <vector>

struct SSelectorType
{
    int Type = 0;
    std::vector<int> Elemental;
    int error = 0;
};

struct APData
{
    std::vector<int> buildingName;
    float latitude=0;
    float longitude = 0;
    int floorInfo = 0;
    int ApartIndex = 0;
};

struct SaleData
{
    int articleNo;
    int dealOrWarrantPrc;
    std::vector<int> tradeTypeName;
    std::vector<int> realtorName;
    int floorInfo;
    int ApartIndex;
};