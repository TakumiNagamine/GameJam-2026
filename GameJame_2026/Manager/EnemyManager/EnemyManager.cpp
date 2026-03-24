#include "EnemyManager.h"
#include "DxLib.h"
#include <cstdlib>
#include <ctime>

// ======================================================
// コンストラクタ
// ======================================================
EnemyManager::EnemyManager()
{
}

// ======================================================
// 初期化
// ======================================================
void EnemyManager::Initialize()
{
    std::srand((unsigned int)std::time(nullptr));
    //合計5体のEnemyが存在(上限5体？)
    enemies.resize(maxEnemies);

    // Enemyの生成座標の代入
    enemy_pos = {
        {1200,140},
        {1200,230},
        {1200,310},
        {1200,400},
        {1200,480},
        {1200,570}
    };


    for (auto& e : enemies)
        e.Initialize();
}

void EnemyManager::Update()
{
    int aliveCount = 0;

    // 生存数カウント
    for (auto& e : enemies)
        if (e.is_alive) aliveCount++;

    // 足りなければ生成
    while (aliveCount < maxEnemies)
    {
        for (auto& e : enemies)
        {
            if (!e.is_alive)
            {
                int index = GetRand(enemy_pos.size() - 1);

                //e.SpawnRandom();
                e.Spawn(enemy_pos[index].x, enemy_pos[index].y);
                aliveCount++;
                break;
            }
        }
    }

    // 移動処理
    for (auto& e : enemies)
    {
        if (!e.is_alive) continue;

       // Enemyの移動処理
        e.UpdaeMove();
    }
}


// ======================================================
// ★ ユニットとの衝突判定（矩形 × 矩形）
// ======================================================
void EnemyManager::CheckCollisionWithUnits(UnitManager& unitManager,float delta_second)
{
    // Unit一覧を取得
    auto& units = unitManager.GetUnits();
    

    // Enemyを1体ずつ確認、処理
    for (auto& e : enemies)
    {
       
        // 死んでいるEnemyは無視する
        if (!e.is_alive) continue;

        //　Unitを1体ずつ確認、処理
        for (auto& u : units)
        {

            if (CollisionManager::CheckBoxCollision(
                e.location, e.GetCollisionWidth(), e.GetCollisionHight(),
                u.GetLocation(), u.GetCollisionWidth(), u.GetCollisionHight()))
            {

                // Unitの前で停止する
                //e.speed = 0.0f;

                // Enemyの速度更新処理
                e.UpdateSpeed(u.hp);

                //Enemyのバトル処理(unitの攻撃を受け取る)
                e.UpdateBattle(u.GetDamage(),delta_second);

                // ユニットにダメージ
               // u.Damage(1);

                break; // 1体と当たったら次の敵へ
            }
            else
            {
                
            }
        }
    }
}


void EnemyManager::CheckCollisionWithBase(Base* base)
{
    for (auto& e : enemies)
    {
        if (CollisionManager::CheckBoxCollision(
            e.location, e.GetCollisionWidth(), e.GetCollisionHight(),
            base->GetLocation(), base->GetWidth(), base->GetHeight()))
        {
            base->TakeDamage(1);
            e.DeleteEnemy();
        }
    }
}

void EnemyManager::CheckDeadEnemies(CostManager& costManager)
{
    for (auto& e : enemies)
    {
        if (!e.is_alive && !e.reward_given)
        {
            costManager.AddMoney(100);
            e.reward_given = true;
        }
    }
}

void EnemyManager::TimeUpdate(float delta_second)
{


}

// ======================================================
// 描画
// ======================================================
void EnemyManager::Draw() const
{
    for (const auto& e : enemies)
    {
        if (!e.is_alive) continue;

        e.Draw();

        //DrawExtendFormatString(50, e.location.y, 1.0, 1.0, GetColor(255, 255, 255), "%d", e.hp);
        
    }
}

std::vector<Enemy>& EnemyManager::GetEnemies()
{
    return enemies;
}

void EnemyManager::SetDebugCollision(bool flag)
{
    debugDrawCollision = flag;
}

int EnemyManager::GetDamage() const
{
    for (auto& e : enemies)
    {
        return e.GetDamage();
    }
}
