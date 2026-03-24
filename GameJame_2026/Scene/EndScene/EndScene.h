#pragma once
#include "../SceneBase/SceneBase.h"

#include "../../Manager/ResourceManager/ResourceManager.h"

// 前方宣言
class Fade;

/*
    エンドシーンクラス

    ・ゲーム終了演出を行う
    ・フェードイン → 一定時間表示 → フェードアウト
    ・フェードアウト完了後にアプリ終了
*/
class EndScene : public SceneBase
{
private:
    Fade* fade;            // フェード制御クラス

    int display_time;      // 表示時間カウント（フレーム単位）
    bool is_fade_out;      // フェードアウト中かどうか

    ResourceManager* resource;

    //エンド画面の背景や文字画像など
    int team_name;          // チーム名の画像
    int member_name;        // メンバーの画像
    int thank_U;            // thankyouforplayingの画像
    int illust;             // イラスト全般 の画像
    int illust_name;        // イラストを描いてくれた人 の画像
    int BGM_SE;             // 音源元の画像
    int audiostock;         // 音源元の名前画像
    int back_img;         // 音源元の名前画像

public:
    EndScene();
    virtual ~EndScene();

    void Initialize() override;     // 初期化処理
    eSceneType Update(float delta_second) override;   // 更新処理
    void Draw() const override;     // 描画処理
    void Finalize() override;       // 終了処理

    eSceneType GetNowSceneType() const override;
};
