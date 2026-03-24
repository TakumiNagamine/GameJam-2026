#pragma once
#include "../../Vector2D/Vector2D.h"
#include "../Base/Base.h"
#include "../../Manager/CollisionManager/CollisionManager.h"
#include "../../Manager/ResourceManager/ResourceManager.h"

// ======================================================
// Enemyクラス
// ・グリッドに沿ってスムーズに移動する敵
// ・中心基準の矩形当たり判定を持つ
// ======================================================
class Enemy
{
public:
    enum class Lane
    {
        Row0, Row1, Row2, Row3, Row4, Row5
    };

    bool is_alive;          // 生存判定
    Vector2D location;      // 座標


    bool reward_given;

    int hp;                 // 体力
    int attack_power;       // 攻撃力

    float speed;            // 進行速度
    float original_speed;    // 進行速度（受け渡し用）
   

private:

    ResourceManager* resource;	 // リソースマネージャー

    int img;                // 画像

    // ===== 当たり判定サイズ =====
    float collisionWidth;
    float collisionHight;

    //時間管理
    float time;
    float interval;         // 待機時間

public:
    Enemy();

    void Initialize();

    void Spawn(Lane laneType);
    void SpawnRandom();
    // speedのランダム生成
    void SpeedRandom();

    // enemyの削除
    void DeleteEnemy();

    /// <summary>
    /// Unitとの戦闘処理
    /// </summary>
    /// <param name="unit_damege">unitの攻撃力</param>
    void UpdateBattle(int unit_damage,float delta_second);

    /// <summary>
    /// 攻撃力のクールダウン設定
    /// </summary>
    /// <param name="delta_second">1フレームにかかる時間</param>
    void UpdateAttackCoolDown(float delta_second);

    /// <summary>
    /// スピードの更新処理
    /// </summary>
    /// <param name="unit_hp">unitの体力</param>
    void UpdateSpeed(int unit_hp);


    // 移動処理
    void UpdaeMove();

    /// <summary>
    /// Enemyの生成処理
    /// </summary>
    /// <param name="x">x座標</param>
    /// <param name="y">y座標</param>
    void Spawn(float x, float y);

    void Draw() const;

    // ===== 当たり判定取得 =====
    float GetCollisionWidth() const { return collisionWidth; }
    float GetCollisionHight() const { return collisionHight; }
    
    /// <summary>
    /// 攻撃力を返す
    /// </summary>
    /// <returns>enemyの攻撃力</returns>
    int GetDamage() const { return attack_power; }

};