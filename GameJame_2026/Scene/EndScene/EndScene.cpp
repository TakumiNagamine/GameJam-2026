#include "EndScene.h"
#include "../../Fade/Fade.h"
#include "DxLib.h"

//--------------------------------------
// コンストラクタ
//--------------------------------------
EndScene::EndScene()
    : fade(nullptr)
    , display_time(0)
    , is_fade_out(false)
    , team_name{}
    , member_name{}
    , thank_U{}
    , illust{}
    , illust_name{}
    , BGM_SE{}
    , audiostock{}
    , back_img{}
{
}

//--------------------------------------
// デストラクタ
//--------------------------------------
EndScene::~EndScene()
{
}

//--------------------------------------
// 初期化処理
//--------------------------------------
void EndScene::Initialize()
{
    // 基底クラス初期化（GameObject管理など）
    SceneBase::Initialize();

    // フェードクラス生成
    fade = new Fade();

    // フェードイン開始
    fade->Initialize(true);

    // 表示時間リセット
    display_time = 0;
    // フェードアウト状態リセット
    is_fade_out = false;

    resource = ResourceManager::GetInstance();

    team_name = LoadGraph("Resource/images/team_name.png");
    member_name = LoadGraph("Resource/images/name_2.png");
    thank_U = LoadGraph("Resource/images/thank_U_2.png");
    illust = LoadGraph("Resource/images/illust_all_2.png");
    illust_name = LoadGraph("Resource/images/illust_name.png");
    BGM_SE = LoadGraph("Resource/images/BGM_SE_2.png");
    audiostock = LoadGraph("Resource/images/audiostock_2.png");

    back_img = LoadGraph("Resource/images/title_back_img.png");


}

//--------------------------------------
// 更新処理
//--------------------------------------
eSceneType EndScene::Update(float delta_second)
{
    // GameObject更新
    SceneBase::Update(delta_second);

    // フェード更新（毎フレーム必須）
    fade->Update();

    // フェードアウト中でなければ時間をカウント
    if (!is_fade_out)
    {
        // フェードイン完了後からカウント開始
        if (fade->GetEndFlag())
        {
            display_time++;

            // 約5秒表示（60fps想定）
            if (display_time >= 300)
            {
                // フェードアウト開始
                fade->Initialize(false);
                is_fade_out = true;
            }
        }
    }

    // フェードアウト完了後にアプリ終了
    if (is_fade_out && fade->GetEndFlag())
    {
        DxLib_End();   // DxLib終了処理
        exit(0);       // アプリ終了
    }

    // Endは遷移しないため常に自分を返す
    return eSceneType::eEnd;
}

//--------------------------------------
// 描画処理
//--------------------------------------
void EndScene::Draw() const
{
    // 画面クリア
    ClearDrawScreen();

    unsigned int white = GetColor(255, 255, 255);

    //背景画像

    DrawGraphF(0.0,0.0, back_img, TRUE);

    //// エンドメッセージ表示
    //SetFontSize(DEFAULT_FONT_SIZE);
    //DrawFormatString(0, 20, white, "エンド画面です。");

    /*SetFontSize(50);
    DrawFormatString(350, 350, white, "Thenk You for playing!");*/
    
    DrawGraph(100, 40, team_name, TRUE);
    DrawGraph(150, 400, thank_U, TRUE);
    DrawGraph(10, 550, BGM_SE, TRUE);
    DrawGraph(800, 550, illust, TRUE);
     
  /*  DrawFormatString(150, 550, white, "BGM・SE");
    DrawFormatString(900, 550, white, "イラスト全般");
    SetFontSize(40);*/
    //DrawFormatString(150, 130, white, "長嶺　匠　　　後真地　萌香　　　桑江　友太\n仲村渠　唯　　仲村　拓海");
    
    DrawGraph(100, 130, member_name, TRUE);
    DrawGraph(130, 620, audiostock, TRUE);
    DrawGraph(850, 620, illust_name, TRUE);

    //DrawFormatString(130, 650, white, "Audiostock");
    //DrawFormatString(950, 650, white, "仲村　拓海");

    // GameObject描画（使用していれば）
    SceneBase::Draw();

    // フェード描画（最後に描く）
    fade->Draw();
}

//--------------------------------------
// 終了処理
//--------------------------------------
void EndScene::Finalize()
{
    // フェード解放
    delete fade;
    fade = nullptr;

    // 基底クラス終了処理
    SceneBase::Finalize();
}

//--------------------------------------
// 現在のシーンを返す
//--------------------------------------
eSceneType EndScene::GetNowSceneType() const
{
    return eSceneType::eEnd;
}
