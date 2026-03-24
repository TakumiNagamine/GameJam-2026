#pragma once
#include <vector>
#include "../../Object/Enemy/Enemy.h"
#include "../UnitManager/UnitManager.h"
#include "../CollisionManager/CollisionManager.h"
#include "../CostManager/CostManager.h"

class UnitManager;

// ======================================================
// EnemyManager
// 敵の生成・更新・描画・衝突管理
// ======================================================
class EnemyManager
{
private:
    std::vector<Enemy> enemies;
    const int maxEnemies = 5;               // Enemyの生成上限

    std::vector<Vector2D> enemy_pos;        // Enemyの生成座標管理,可変長

    bool debugDrawCollision = true;
public:
    EnemyManager();

    void Initialize();

    void Update();

    // ★ 修正
    /// <summary>
    /// EnemyとUnitの当たり判定処理
    /// </summary>
    /// <param name="unitManager">unitの情報</param>
    /// <param name="delta_second">１フレームにかかった時間</param>
    void CheckCollisionWithUnits(UnitManager& unitManager,float delta_second);

    /// <summary>
    /// EnemyとBaseの当たり判定の計算
    /// </summary>
    /// <param name="base_pos">Baseの中心座標</param>
    /// <param name="width">Baseの横幅</param>
    /// <param name="hight">Baseの縦幅</param>
    void CheckCollisionWithBase(Base* base);

    /// <summary>
    /// 時間の更新処理
    /// </summary>
    void TimeUpdate(float delta_second);

    void CheckDeadEnemies(CostManager& costManager);

    void Draw() const;

    std::vector<Enemy>& GetEnemies();

    void SetDebugCollision(bool flag);

    int GetDamage()const;
};