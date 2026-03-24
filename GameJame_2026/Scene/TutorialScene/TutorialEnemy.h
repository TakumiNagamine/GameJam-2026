#pragma once
#include "../SceneBase/SceneBase.h"
#include "../../Fade/Fade.h"
#include "../../InputControl/InputControl.h"
#include "../../Manager/ResourceManager/ResourceManager.h"


// ===============================
// エネミーチュートリアルシーン
// ===============================
class TutorialEnemyScene : public SceneBase
{
private:
    Fade* fade;
    eSceneType next_scene;
    bool enter_old;

    ResourceManager* resource;

    int cursor_move_se;         // カーソルSE
    int cursor_push_se;         // カーソル決定SE

    bool stick_up_old;           // 前フレームのスティック上状態
    bool stick_down_old;         // 前フレームのスティック下状態

    int back_img;               // 背景画像


public:
    TutorialEnemyScene();
    ~TutorialEnemyScene();

    void Initialize() override;
    eSceneType Update(float delta_second) override;
    void Draw() const override;
    void Finalize() override;

    eSceneType GetNowSceneType() const override;
};