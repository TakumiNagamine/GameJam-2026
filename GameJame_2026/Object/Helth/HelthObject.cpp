#include "../Helth/HelthObject.h"

// 最大HPを設定し、現在HPを最大値で初期化する。
HealthObject::HealthObject(int maxHp)
    : m_maxHp(maxHp), m_hp(maxHp)
{
}

// ダメージを受けてHPを減少させる。0未満にはならないよう補正する。
void HealthObject::TakeDamage(int damage)
{
    m_hp -= damage;
    if (m_hp < 0) m_hp = 0;
}

// HPを回復する。最大HPを超えないよう補正する。
void HealthObject::Heal(int amount)
{
    m_hp += amount;
    if (m_hp > m_maxHp) m_hp = m_maxHp;
}

// HPが0以下なら死亡と判定する。
bool HealthObject::IsDead() const
{
    return m_hp <= 0;
}

int HealthObject::GetHP() const { return m_hp; }
int HealthObject::GetMaxHP() const { return m_maxHp; }
