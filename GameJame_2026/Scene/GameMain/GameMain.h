#pragma once
#include "../SceneBase/SceneBase.h"
#include "../../Object/Base/Base.h"
#include "../../Fade/Fade.h"
#include "../../InputControl/InputControl.h"
#include "../../Manager/EnemyManager/EnemyManager.h"
#include "../../Manager/UnitManager/UnitManager.h"
#include "../../Manager/CostManager/CostManager.h"
#include "../../Manager/ResourceManager/ResourceManager.h"

// ======================================
// メインゲームシーン
// ======================================
class GameMain : public SceneBase
{
private:

    // フェード
    Fade* fade;
    eSceneType next_scene;
    bool enter_old;

    EnemyManager enemyManager;      // 敵管理
    UnitManager unitManager;        // 味方管理
    CostManager costManager;        // コスト管理
    Base* base;                     // 城管理

    ResourceManager* resource;      // リソースマネージャー

    int logo_image;           // 陣営アイコン画像
    //int background_image;           // 画像
    //int background_image;           // 画像
    //int background_image;           // 画像
    //int background_image;           // 画像
    //int background_image;           // 画像

    // 画面
    static const int SCREEN_W = 1280;
    static const int SCREEN_H = 720;

    // グリッド
    static const int GRID_COL = 8;       // 横のマス目数
    static const int GRID_ROW = 6;       // 縦のマス目数
    static const int CELL_SIZE = 120;    // マス目サイズ

    // グリッド開始位置
    int gridStartX;
    int gridStartY;

    int back_img;                   // 背景画像

    int baseHP;                     // 基地HP

    float frameCount;                 // 制限時間

    int bgm;

public:
    GameMain();
    ~GameMain();

    void Initialize() override;
    eSceneType Update(float delta_second) override;
    void Draw() const override;
    void Finalize() override;

    void GridDraw() const;

    eSceneType GetNowSceneType() const override;
};