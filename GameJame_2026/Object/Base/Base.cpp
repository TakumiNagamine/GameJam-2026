#include "Base.h"
#include "DxLib.h"

Base::Base()
    : width()
    , hight()
    , center_pos{}
{
}

void Base::Initialize()
{
    location.x = 200;
    location.y = 100;
    width = 80;
    hight = 500;

    hp = 10;

    //矩形の中心座標の格納
    center_pos.x = location.x + width / 2;
    center_pos.y = location.y + hight / 2;
}

void Base::Draw() const
{
   /* DrawBox((int)location.x, (int)location.y,
        (int)location.x + width,
        (int)location.y + hight,
        GetColor(0, 200, 0), TRUE);*/
}

// ダメージ処理
void Base::TakeDamage(int damage)
{
    hp -= damage;

    if (hp < 0)
    {
        hp = 0;
    }
}

int Base::GetHP() const
{
    return hp;
}

// 0になったとき処理
bool Base::IsDead() const
{
    return hp <= 0;
}