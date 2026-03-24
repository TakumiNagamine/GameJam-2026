#pragma once

enum class UnitType
{
    Character1,   // 300コストのキャラ
    Character2,   // 700コストのキャラ
    Character3    // 1000コストのキャラ
};

inline int GetUnitCost(UnitType type)
{
    switch (type)
    {
    case UnitType::Character1: return 300;
    case UnitType::Character2:  return 700;
    case UnitType::Character3:    return 1000;
    }
    return 300;
}