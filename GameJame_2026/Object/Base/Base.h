#pragma once
#include "../GameObject/GameObject.h"

// ===============================
// Šî’nƒNƒ‰ƒX
// ===============================
class Base : public GameObject
{
private:

    Vector2D center_pos;

    float width;
    float hight;

    int hp;

public:
    Base();
    void Initialize() override;
    void Draw() const override;
    void TakeDamage(int damage);
 

    Vector2D GetLocation() { return center_pos; }
    int GetWidth() { return width; }
    int GetHeight() { return hight; }
    int GetHP() const;
    bool IsDead() const;
};