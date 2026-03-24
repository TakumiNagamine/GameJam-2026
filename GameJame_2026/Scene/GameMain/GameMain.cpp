#include "GameMain.h"
#include "DxLib.h"
#include "../../Manager/Color.h"


// コンストラクタ
GameMain::GameMain()
    : fade(nullptr)
    , next_scene(eSceneType::eInGame)
    , enter_old(false)
    , baseHP(10)
    , frameCount(0)
    , back_img{}
    , bgm{}
{
}

// デストラクタ
GameMain::~GameMain()
{
}

// 初期化処理
void GameMain::Initialize()
{
    resource = ResourceManager::GetInstance();

   
    fade = new Fade();
    fade->Initialize(true);

    next_scene = eSceneType::eInGame;
    enter_old = false;

    // グリッド中央配置
    int gridWidth = GRID_COL * CELL_SIZE;
    int gridHeight = GRID_ROW * CELL_SIZE;

    /* gridStartX = (SCREEN_W - gridWidth) / 2;
     gridStartY = (SCREEN_H - gridHeight) / 2;*/

    gridStartX = 320;
    gridStartY = 180;

    // 基地配置
    int baseY = gridStartY + (GRID_ROW / 2) * CELL_SIZE + CELL_SIZE / 2;
    //base = CreateObject<Base>(Vector2D(gridStartX - 60, baseY));
    base = CreateObject<Base>(Vector2D(0.0, 0.0));

    // 背景画像挿入
    back_img = resource->GetImages("Resource/images/game_main_2.png", 1, 0, 0, 0, 0)[0];

    // 陣営ロゴ
    logo_image = resource->GetImages("Resource/images/icon_.png", 1, 0, 0, 0, 0)[0];

    bgm = resource->GetSounds("Resource/Sounds/BGM/game_main_prov.mp3");

    PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
    
    // コストマネージャをUnitManagerに渡す
    unitManager.SetCostManager(&costManager);

    // 敵初期化
    enemyManager.Initialize();

    // 味方初期化
    unitManager.Initialize();
    unitManager.SetGridInfo(
        gridStartX,
        gridStartY,
        CELL_SIZE,
        GRID_COL,
        GRID_ROW
    );

    // コスト初期化
    costManager.Initialize();

    baseHP = 10;
    //frameCount = 0;
}

// 更新処理
eSceneType GameMain::Update(float delta_second)
{

    // タイマーの更新
    frameCount += delta_second;

    // 時間でコスト増加
    costManager.Update(delta_second);

    // ユニット更新（カーソル含む）
    unitManager.Update(enemyManager.GetDamage(), delta_second);

    InputControl* input = InputControl::GetInstance();
    input->Update();

    // フェード
    if (next_scene != eSceneType::eInGame)
    {
        fade->Update();

        if (fade->GetEndFlag())
        {
            return next_scene;
        }

        return eSceneType::eInGame;
    }

    // 敵更新(生成)
    enemyManager.Update();

    // EnemyとBaseの当たり判定処理
    enemyManager.CheckCollisionWithBase(base );

    // EnemyとUnitの当たり判定処理
    enemyManager.CheckCollisionWithUnits(
        unitManager, delta_second
    );

    // UnitがEnemyに当たった時の判定
    unitManager.CheckCollisionWithEnemies(
        enemyManager, delta_second
    );

    // 敵撃破でお金
    enemyManager.CheckDeadEnemies(costManager);


    // デバック用（Resultシーンに遷移）
    if (input->GetButtonInputState(KEY_INPUT_A) == ePadInputState::ePress)
    {
        return eSceneType::eClear;
    }

    // 防衛失敗遷移
    if (base->IsDead())
    {
        // シーン遷移
        return eSceneType::eMiss;
    }

    // 防衛成功遷移
    if (frameCount >= 30.0f && !base->IsDead())
    {
        return eSceneType::eClear;
    }


    SceneBase::Update(delta_second);
    fade->Update();

    return eSceneType::eInGame;
}

// 描画処理
void GameMain::Draw() const
{
    ClearDrawScreen();

    // 背景画像の描画
    DrawRotaGraphF(640.0, 365.0, 1.0, 0.0, back_img, TRUE);

    //// ロゴUI
    //DrawExtendGraph(1140, 340, 140, 140, logo_image, TRUE);

    // UI
    /*DrawBox(20, 20, 140, 140, GetColor(60, 60, 60), TRUE);
    DrawString(40, 70, "ICON", C_BLACK);*/

    //DrawString(180, 40, "敵の侵入数:", C_BLACK);
    //DrawFormatString(290, 40, C_BLACK, "%d", baseHP);

    // HPの描画
    //DrawFormatString(50, 615,GetColor(255, 255, 255),"Base HP : %d",base->GetHP());

    // HPの描画
    DrawExtendFormatString(30, 610, 1.5, 1.5, C_WHITE, "Base HP : %d", base->GetHP());

    // タイマーの表示
    DrawExtendFormatString(80, 180, 3.0, 3.0, C_WHITE, "%d", (int)frameCount);

    // 下UI
    /*DrawBox(
        0,
        SCREEN_H - 120,
        SCREEN_W,
        SCREEN_H,
        GetColor(40, 40, 40),
        TRUE
    );*/

    // コストの描画
    DrawExtendFormatString(35, 675, 1.5, 1.5, C_YELLOW, "Cost : %d", costManager.GetMoney());

    // グリッド描画
    GridDraw();

    // オブジェクト
    SceneBase::Draw();
    enemyManager.Draw();
    unitManager.Draw();
    fade->Draw();
}

// 終了時処理
void GameMain::Finalize()
{
    StopSoundMem(bgm);
    delete fade;
    fade = nullptr;
}

// グリッド線（マス目（台形寄り））描画処理
void GameMain::GridDraw() const
{
    // グリッド設定
    static const int GRID_COL = 8;
    static const int GRID_ROW = 6;
    static const int CELL_SIZE = 80;

    // 表示スケール
    float gridScaleX = 1.25f;  // 横の広がり
    float gridScaleY = 1.15f;  // 奥行きの伸び

    // 配置開始位置 
    int gridStartX = 270;
    int gridStartY = 50;

    // 台形調整
    int trapezoidTopInset = 40;
    int trapezoidTopOffsetY = 40;

    // 実際の表示サイズ
    float gridWidth = GRID_COL * CELL_SIZE * gridScaleX;
    float gridHeight = GRID_ROW * CELL_SIZE * gridScaleY;

    // 台形4頂点

    // 上辺
    int topLeftX = gridStartX + trapezoidTopInset;
    int topRightX = gridStartX + (int)gridWidth - trapezoidTopInset;
    int topY = gridStartY + trapezoidTopOffsetY;

    // 下辺
    int bottomLeftX = gridStartX;
    int bottomRightX = gridStartX + (int)gridWidth;
    int bottomY = gridStartY + (int)gridHeight;

    unsigned int col = GetColor(80, 80, 80);


    // 横ライン
    for (int y = 0; y <= GRID_ROW; y++)
    {
        float t = (float)y / GRID_ROW;

        int leftX = (int)(topLeftX + (bottomLeftX - topLeftX) * t);
        int rightX = (int)(topRightX + (bottomRightX - topRightX) * t);
        int posY = (int)(topY + (bottomY - topY) * t);

        DrawLine(leftX, posY, rightX, posY, col);
    }

    // 縦ライン
    for (int x = 0; x <= GRID_COL; x++)
    {
        float t = (float)x / GRID_COL;

        int topX = (int)(topLeftX + (topRightX - topLeftX) * t);
        int bottomX = (int)(bottomLeftX + (bottomRightX - bottomLeftX) * t);

        DrawLine(topX, topY, bottomX, bottomY, col);
    }

}

// 現在シーン
eSceneType GameMain::GetNowSceneType() const
{
    return eSceneType::eInGame;
}