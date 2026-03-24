#include "../ResultScene/ResultMissScene.h"
#include "DxLib.h"

// コンストラクタ
ResultMissScene::ResultMissScene()
    : fade(nullptr)
    , next_scene(eSceneType::eMiss)
    , enter_old(false)
    , back_img{}
{
}

// デストラクタ
ResultMissScene::~ResultMissScene()
{
}

// 初期化処理
void ResultMissScene::Initialize()
{
    fade = new Fade();
    fade->Initialize(true);

    next_scene = eSceneType::eMiss;
    enter_old = false;

    resource = ResourceManager::GetInstance();


    result_gameover_bgm = resource->GetSounds("Resource/sounds/BGM/result_gameover_prov.mp3");
    cursor_move_se = resource->GetSounds("Resource/sounds/SE/cursor.mp3");
    cursor_push_se = resource->GetSounds("Resource/sounds/SE/cursor_push.mp3");

    back_img = resource->GetImages("Resource/images/rizaruto_22.jpg", 1, 0, 0, 0, 0)[0];

    PlaySoundMem(result_gameover_bgm, DX_PLAYTYPE_BACK);
}

// 更新処理
eSceneType ResultMissScene::Update(float delta_second)
{
    InputControl* input = InputControl::GetInstance();

    // フェードアウト中処理
    if (next_scene != eSceneType::eMiss)
    {
        fade->Update();

        if (fade->GetEndFlag())
        {
            return next_scene;
        }

        return eSceneType::eMiss;
    }

    // Enter瞬間入力
    bool enter_now = CheckHitKey(KEY_INPUT_RETURN);
    bool enter_press = (enter_now && !enter_old);
    enter_old = enter_now;

    // Aボタン or Enterでタイトルへ戻る
    if (input->GetButtonInputState(XINPUT_BUTTON_A) == ePadInputState::ePress
        || enter_press)
    {
        PlaySoundMem(cursor_push_se, DX_PLAYTYPE_BACK);
        StopSoundMem(result_gameover_bgm);

        fade->Initialize(false);
        next_scene = eSceneType::eTitle;
    }

    fade->Update();

    return eSceneType::eMiss;
}

// 描画処理
void ResultMissScene::Draw() const
{
    ClearDrawScreen();

    unsigned int white = GetColor(255, 255, 255);

    DrawFormatString(20, 20, white, "リザルト失敗画面です");
    DrawFormatString(20, 50, white, "Aボタン または Enterキーでタイトルへ戻る");

    DrawRotaGraphF(640.0, 360.0, 1.0, 0.0, back_img, TRUE);


    fade->Draw();
}

// 終了処理
void ResultMissScene::Finalize()
{
    delete fade;
    fade = nullptr;
}

// 現在シーン
eSceneType ResultMissScene::GetNowSceneType() const
{
    return eSceneType::eMiss;
}
