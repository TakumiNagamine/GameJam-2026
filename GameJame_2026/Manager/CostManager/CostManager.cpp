#include "../CostManager/CostManager.h"

CostManager::CostManager()
{
    Initialize();
}

void CostManager::Initialize()
{
    money = 0;
    //money = 800000;
    timer = 0.0f;
}

void CostManager::Update(float delta_second)
{
    timer += delta_second;

    // 1•bŒo‰ß‚µ‚½‚ç
    if (timer >= 1.0f)
    {
        // ƒRƒCƒ“‚ğ100‘‚â‚·
        money += 100;
        timer = 0.0f;
    }
}

void CostManager::AddMoney(int amount)
{
    money += amount;
}

bool CostManager::UseMoney(int cost)
{
    if (money < cost) return false;

    money -= cost;
    return true;
}

int CostManager::GetMoney() const
{
    return money;
}