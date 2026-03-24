#pragma once
#include "../../Object/GameObject/GameObject.h"

// HPを持つオブジェクトの基底クラス。
// PlayerやEnemyなど「破壊・生存判定」が必要なオブジェクトで共通利用する。
class HealthObject : public GameObject
{
protected:
    int m_hp;     // 現在HP
    int m_maxHp;  // 最大HP

public:
    // 最大HPを設定し、現在HPを最大値で初期化する。
    HealthObject(int maxHp);
    virtual ~HealthObject() {}

    // 指定ダメージ分HPを減少させる。HPは0未満にならない。
    virtual void TakeDamage(int damage);

    // 指定量HPを回復する。HPは最大値を超えない。
    virtual void Heal(int amount);

    // HPが0以下ならtrueを返す（死亡判定）。
    bool IsDead() const;

    int GetHP() const;
    int GetMaxHP() const;
};
