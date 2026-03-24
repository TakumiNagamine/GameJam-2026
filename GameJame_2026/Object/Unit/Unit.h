#pragma once
#include "../../Vector2D/Vector2D.h"
#include "../../Manager/CollisionManager/CollisionManager.h"
#include "../../Manager/ResourceManager/ResourceManager.h"
#include "../Unit/UnitType.h"

#define UNIT1 (0)
#define UNIT1_ATTACK (1)
#define UNIT2 (2)
#define UNIT2_ATTACK (3)

// ======================================================
// Playerクラス
// ・移動
// ・HP管理
// ・中心基準の矩形当たり判定を持つ
// ======================================================
class Unit
{
public:
    bool is_alive;          // 生きているか？死んでいるか？
    Vector2D location;      // 座標

    float speed;            // 速度    
    int hp;                 // 体力

    int attack_power;       // 攻撃力

    // aho
private:
    UnitType type;          // Unitの種類管理
    ResourceManager* resource;	 // リソースマネージャー

    int img[4];                // 画像（立ち絵、攻撃）

    bool attack;            // 攻撃フラッグ
    int attack_se;          // 攻撃時SE

    int imageHandle;        // このUnitの立ち絵



    // ===== 当たり判定サイズ =====
    float collisionWidth;   // 横幅
    float collisionHight;  // 縦幅

    float time;             //時間管理
    float interval;         // 攻撃間隔

public:
    Unit(UnitType t);

    void Initialize(float x,float y);

    void Update(int enemy_damage,float delta_second);

    /// <summary>
    /// Enemyとの戦闘処理
    /// </summary>
    /// <param name="unit_damege"></param>
    void UpdateBattle(int enemy_damege,float delta_secon);

    /// <summary>
    /// 攻撃力のクールダウン設定
    /// </summary>
    /// <param name="delta_second">1フレームにかかる時間</param>
    void UpdateAttackCoolDown(float delta_second);

    void Draw() const;

    // ===== HP関連 =====
    void Damage(int amount);
    int GetHP() const { return hp; }
    void DeleteUnit();

    // ===== 当たり判定取得 =====
    Vector2D GetLocation() const { return location; }
    float GetCollisionWidth() const { return collisionWidth; }
    float GetCollisionHight() const { return collisionHight; }

    // 攻撃ステータスのリセット
    void ResetAttack();

    /// <summary>
    /// 攻撃力を返す
    /// </summary>
    /// <returns>unitの攻撃力</returns>
    int GetDamage() const { return attack_power; }
};