#include "TutorialCompany.h"
#include "DxLib.h"

// ===============================
TutorialCompanyScene::TutorialCompanyScene()
    : fade(nullptr)
    , next_scene(eSceneType::eTutorialCompany)
    , enter_old(false)
    , cursor_move_se{}
    , cursor_push_se{}
    , stick_up_old(false)
    , stick_down_old(false)
    , back_img{}


{
}

TutorialCompanyScene::~TutorialCompanyScene()
{
}

// ===============================
void TutorialCompanyScene::Initialize()
{
    fade = new Fade();
    fade->Initialize(true);

    resource = ResourceManager::GetInstance();

    next_scene = eSceneType::eTutorialCompany;
    enter_old = false;

    cursor_move_se = resource->GetSounds("Resource/sounds/SE/cursor_tutorial.mp3");
    cursor_push_se = resource->GetSounds("Resource/sounds/SE/cursor_push.mp3");

    back_img = resource->GetImages("Resource/images/icon_23.png",1,0,0,0,0)[0];

    ChangeNextPlayVolumeSoundMem(180, cursor_move_se);
    ChangeNextPlayVolumeSoundMem(150, cursor_push_se);
}

// ===============================
eSceneType TutorialCompanyScene::Update(float delta_second)
{
    InputControl* input = InputControl::GetInstance();

    // フェードアウト中
    if (next_scene != eSceneType::eTutorialCompany)
    {
        fade->Update();

        if (fade->GetEndFlag())
        {
            return next_scene;
        }

        return eSceneType::eTutorialCompany;
    }

    // //===============================
    // //スティック瞬間入力化
    // //===============================
    //Vector2D stick = input->GetLeftStickTiltPercentage();

    //bool stick_up_now = (stick.y > 0.5f);
    //bool stick_down_now = (stick.y < -0.5f);

    //bool stick_up_press = (stick_up_now && !stick_up_old);
    //bool stick_down_press = (stick_down_now && !stick_down_old);

    //stick_up_old = stick_up_now;
    //stick_down_old = stick_down_now;

    // Enter瞬間入力
    bool enter_now = CheckHitKey(KEY_INPUT_RETURN);
    bool enter_press = (enter_now && !enter_old);
    enter_old = enter_now;

    // スティックを右 or Enterで次のページへ
    if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_RIGHT) == ePadInputState::ePress
        || enter_press)
    {
        fade->Initialize(false);
        next_scene = eSceneType::eTutorialCharacter;

        PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK);
    }

    //左スティックで敵キャラクター説明へ
    if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_LEFT) == ePadInputState::ePress
        || enter_press)
    {
        fade->Initialize(false);
        next_scene = eSceneType::eTutorial;

        PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK);
    }

    // Aボタン or Enterでタイトルへ
    if (input->GetButtonInputState(XINPUT_BUTTON_A) == ePadInputState::ePress
        || enter_press)
    {
        fade->Initialize(false);
        next_scene = eSceneType::eTitle;

        PlaySoundMem(cursor_push_se, DX_PLAYTYPE_BACK);
    }

    fade->Update();

    return eSceneType::eTutorialCompany;
}

// ===============================
void TutorialCompanyScene::Draw() const
{
    ClearDrawScreen();

    unsigned int white = GetColor(255, 255, 255);

    DrawFormatString(20, 20, white, "会社概要画面です");
    DrawFormatString(20, 40, white, "右十字キー でキャラクター説明に遷移");
    DrawFormatString(20, 60, white, "左十字キー で操作説明に遷移");
    DrawFormatString(20, 80, white, "Aボタン でタイトルに遷移");

    // 背景画像
    DrawRotaGraphF(640.0, 360.0, 1.0, 0.0, back_img, TRUE);


    fade->Draw();
}

// ===============================
void TutorialCompanyScene::Finalize()
{
    delete fade;
    fade = nullptr;
}

// ===============================
eSceneType TutorialCompanyScene::GetNowSceneType() const
{
    return eSceneType::eTutorialCompany;
}
