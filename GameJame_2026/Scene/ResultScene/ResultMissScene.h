#pragma once
#include "../SceneBase/SceneBase.h"
#include "../../Fade/Fade.h"
#include "../../InputControl/InputControl.h"
#include "../../Manager/ResourceManager/ResourceManager.h"

// ===============================
// リザルトシーン
// ===============================
class ResultMissScene : public SceneBase
{
private:
    Fade* fade;                  // フェード制御
    eSceneType next_scene;       // 次に遷移するシーン
    bool enter_old;              // Enter瞬間入力用

    ResourceManager* resource;      // リソースマネージャー

    int result_gameover_bgm;    // 失敗BGM

    int cursor_move_se;              // カーソルSE
    int cursor_push_se;         // カーソル決定SE

    int back_img;

public:
    ResultMissScene();
    ~ResultMissScene();

    void Initialize() override;
    eSceneType Update(float delta_second) override;
    void Draw() const override;
    void Finalize() override;

    eSceneType GetNowSceneType() const override;
};
