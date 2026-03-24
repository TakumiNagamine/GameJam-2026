#pragma once

class CostManager
{
private:
    int money;
    float timer;

public:
    CostManager();

    void Initialize();
    void Update(float delta_second);

    void AddMoney(int amount);
    bool UseMoney(int cost);

    int GetMoney() const;
};