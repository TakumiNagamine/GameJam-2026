#include "Enemy.h"
#include "DxLib.h"
#include <cstdlib>
#include <cmath>


Enemy::Enemy()
    : speed{}
    , is_alive{}
    , location{}
    , collisionWidth{}
    , collisionHight{}
    , hp{}
    , attack_power{}
    , time{}
    , interval{}
    , original_speed{}
    , img{}
    , resource{}
{
    //Initialize();
}


void Enemy::Initialize()
{
    is_alive = false;

    collisionWidth = 150.0f;   // 円直径相当
    collisionHight = 40.0f;

    reward_given = false;

    //初期速度の設定
    speed = 0.5f;
    //敵の体力の設定
    hp = 10;
    //攻撃力の初期設定
    attack_power = 1;

    time = 0.0f;

    interval = 3.0f;

    resource = ResourceManager::GetInstance();

    img = resource->GetImages("Resource/images/Enemy.png",1,0,0,0,0)[0];
}


void Enemy::Spawn(Lane laneType)
{
    SpeedRandom();

    // 速度の代入
    original_speed = speed;
    
    location.x = 0;
    location.y = 0;
}

void Enemy::Spawn(float x, float y)
{
    is_alive = true;

    location.x = x;
    location.y = y;

    SpeedRandom();
    
    // 速度の代入（記憶）
    original_speed = speed;
}


void Enemy::SpawnRandom()
{
    Lane randomLane = static_cast<Lane>(rand() % 6);
    Spawn(randomLane);
}

void Enemy::SpeedRandom()
{
    //Randomで値を返す(0~3)(合計4択)
    int randx = (int)GetRand(3);
  
    switch (randx)
    {
    case 1:
        //1フレーム当たり0.8ピクセル進む
        speed = 0.5f + (randx * 0.3);
    break;

    case 2:
        //1フレーム当たり1.1ピクセル進む
        speed = 0.5 + (randx * 0.3);
    break;

    case 3:
        //1フレーム当たり1.4ピクセル進む
        speed = 0.5 + (randx * 0.3);
    break;

    default:
        //1フレーム当たり0.5ピクセル進む
        speed = 0.5 + (randx * 0.3);
    break;
    }

}

void Enemy::DeleteEnemy()
{
    //Enemyの削除？
    is_alive = false;
    hp = 10;

}

void Enemy::UpdateBattle(int unit_damage,float delta_second)
{
    if (hp > 0)
    {
        // Unitから攻撃を受ける
        hp -= unit_damage;

        UpdateAttackCoolDown(delta_second);
    }
    else
    {
        //敵を削除（敵の死亡）
        DeleteEnemy();
    }
}

void Enemy::UpdateAttackCoolDown(float delta_second)
{
    //時間の加算
    time += delta_second;

    // (interval)時間経過すると攻撃を実行
    if (time >= interval)
    {
        // Enemyの攻撃力設定
        attack_power = 1;
        // 時間をリセット
        time = 0;
    }
    else
    {
        // 攻撃しない状態
        attack_power = 0;
    }
}

void Enemy::UpdateSpeed(int unit_hp)
{

    if (unit_hp > 0)
    {
        // Unitの前で停止
        speed = 0.0f;
    }
    else
    {
        // 元の速度に直す
        speed = original_speed;
    }
}

void Enemy::UpdaeMove()
{
    if (is_alive)
    {
        location.x -= speed;
    }
   
}

void Enemy::Draw() const
{
    if (!is_alive) return;

    // ===== 当たり判定可視化（矩形） =====
    /*float halfw = collisionWidth * 0.5f;
    float halfh = collisionHight * 0.5f;

    DrawBox(
        (int)(location.x - halfw),
        (int)(location.y - halfh),
        (int)(location.x + halfw),
        (int)(location.y + halfh),
        GetColor(0, 0, 255),
        false
    );*/

    //DrawExtendFormatString(50, 400, 1.0, 1.0, GetColor(255, 255, 255), "%f", time);

    // Enemyの描画
    DrawRotaGraphF(location.x, location.y, 0.09, 0.0, img, TRUE);

}