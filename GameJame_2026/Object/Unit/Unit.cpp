#include "Unit.h"
#include "DxLib.h"
#include <algorithm>

Unit::Unit(UnitType t)
    : attack_power{}
    , time{}
    , interval{}
    , is_alive{}
    , hp{}
    , resource{}
    , img{}
    , attack{}
{
    type = t;
}

void Unit::Initialize(float x, float y)
{
    is_alive = true;
    attack = false;

    location.x = x;
    location.y = y;


    // ===== タイプごとにステータス変更 =====
    switch (type)
    {
    case UnitType::Character1:  // 300で買える
        hp = 5;
        attack_power = 2; // 攻撃数値
        interval = 3.0f;
        
        break;
        break;

    case UnitType::Character2:  // 700で買える
        hp = 7;
        attack_power = 5; // 攻撃数値
        interval = 3.0f;
        break;

    case UnitType::Character3:  // 1000で買える
        hp = 10;
        attack_power = 9; // 攻撃数値
        interval = 3.0f;
        break;
    }

    static bool loaded = false;

    resource = ResourceManager::GetInstance();

    img[UNIT1] = resource->GetImages("Resource/images/Unit.png", 1, 0, 0, 0, 0)[0];
    img[UNIT2] = resource->GetImages("Resource/images/Unit2.png", 1, 0, 0, 0, 0)[0];
    img[UNIT1_ATTACK] = resource->GetImages("Resource/images/Unit_attack.png", 1, 0, 0, 0, 0)[0];
    img[UNIT2_ATTACK] = resource->GetImages("Resource/images/Unit2_attack.png", 1, 0, 0, 0, 0)[0];

    attack_se = resource->GetSounds("Resource/sounds/SE/battle_se.mp3");
    //char_se = resource->GetSounds("Resource/sounds/SE/cursor.mp3");

    
    collisionWidth = 50.0f;
    collisionHight = 50.0f;

    time = 0.0f;

    //interval = 3.0f; // 初期インターバル？
}

void Unit::Update(int enemy_damage,float delta_second)
{
    // 駒なので移動しない
}

void Unit::DeleteUnit()
{
    //Unit削除
    is_alive = false;
    hp = 0;

}

void Unit::ResetAttack()
{
    attack = false;
    attack_power = 0;
    time = 0.0f;

}

void Unit::UpdateBattle(int enemy_damage,float delta_second)
{

    if (hp > 0)
    {
        // Enemyから攻撃を受ける
        hp -= enemy_damage;
       
        UpdateAttackCoolDown(delta_second);
    }
    else if(hp == 0 || hp < 0)
    {
        // Playerキャラ(Unit)を削除(Unitの死亡)
        DeleteUnit();
        
    }

}

// 戦闘処理
void Unit::UpdateAttackCoolDown(float delta_second)
{
    //時間の加算
    time += delta_second;

    //　(interval)時間経過すると攻撃を実行
    if (time >= interval)
    {
        // 攻撃フラッグをTRUEにする
        attack = true;
        // Unitの攻撃力設定
        switch (type)
        {
        case UnitType::Character1:
            attack_power = 2;
            PlaySoundMem(attack_se, DX_PLAYTYPE_BACK);
            break;
        case UnitType::Character2:
            attack_power = 5;
            PlaySoundMem(attack_se, DX_PLAYTYPE_BACK);

            break;
        case UnitType::Character3:
            attack_power = 9;
            PlaySoundMem(attack_se, DX_PLAYTYPE_BACK);
            break;
        }

        // 時間をリセット
        time = 0.0f;

    }
    else
    {
        // 攻撃しない状態
        attack_power = 0;
        // 攻撃フラッグをFALSEにする
        if (time >= 1.0f)
        {
            attack = false;
            //time = 0.0f;
        }
    }
}

void Unit::Draw() const
{
    // 画像描画
    //DrawRotaGraphF(location.x, location.y, 0.09, 0.0, imageHandle, TRUE);

    /*DrawCircle(
        (int)location.x,
        (int)location.y,
        25,
        GetColor(0, 255, 0),
        TRUE
    );*/

    /*DrawGraph(
        (int)(location.x - 32),
        (int)(location.y - 32),
        imageHandle,
        TRUE
    );*/

    /*float halfW = collisionWidth * 0.5f;
    float halfH = collisionHight * 0.5f;*/

   /* DrawBox(
        (int)(location.x - halfW),
        (int)(location.y - halfH),
        (int)(location.x + halfW),
        (int)(location.y + halfH),
        GetColor(255, 255, 0),
        FALSE
    );*/


    //DrawExtendFormatString(50, 300, 1.0, 1.0, GetColor(255, 255, 255), "%f",time);

    // キャラクタータイプによって描画する画像を変更
    switch (type)
    {
    case UnitType::Character3:

        if (attack)
        {
            // Unitの描画(攻撃画像)
            DrawRotaGraphF(location.x + 10.0f, location.y - 40.0f, 0.1, 0.0, img[UNIT2_ATTACK], TRUE);

        }
        else
        {
            // Unitの描画(立ち絵)
            DrawRotaGraphF(location.x + 120.0f, location.y - 40.0f, 0.1, 0.0, img[UNIT2], TRUE);
        }

        break;

    default:

        if (attack)
        {
            // Unitの描画(攻撃画像)
            DrawRotaGraphF(location.x - 40.0f, location.y - 40.0f, 0.1, 0.0, img[UNIT1_ATTACK], TRUE);

        }
        else
        {
            // Unitの描画(立ち絵)
            DrawRotaGraphF(location.x + 60.0f, location.y - 40.0f, 0.1, 0.0, img[UNIT1], TRUE);
        }

        break;
    }

}

void Unit::Damage(int amount)
{
    if (!is_alive) return;

    hp -= amount;

    if (hp <= 0)
    {
        hp = 0;
        is_alive = false;  
    }
}