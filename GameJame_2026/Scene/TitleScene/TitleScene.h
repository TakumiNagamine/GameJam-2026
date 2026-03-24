#pragma once
#include "../SceneBase/SceneBase.h"
#include "../../Manager/ResourceManager/ResourceManager.h"

#define START (0)
#define HELP (1)
#define END (2)

#define PI (3.1415926535897932384626433832795f)

// タイトルシーン
class TitleScene : public SceneBase
{
private:
	class Fade* fade;            // フェードクラス

	ResourceManager* resource;	 // リソースマネージャー

	int background_image;       // 背景画像
	int cursor_image;           // カーソル画像
	int title_img;				// ゲームタイトル画像
	int menu_img[3];			// 操作ボタン画像（スタート、ヘルプ、エンド）
	int title_back_img;			// タイトル背景画像
	
	int menu_cursor;             // 現在選択中メニュー番号
	/*
		0 : GameMain
		1 : Tutorial
		2 : End
	*/

	int title_bgm;               // タイトルBGM
	int cursor_move_se;          // カーソル移動SE
	int cursor_push_se;          // 決定SE

	// 入力補助用
	bool stick_up_old;           // 前フレームのスティック上状態
	bool stick_down_old;         // 前フレームのスティック下状態
	bool enter_old;              // 前フレームのEnter状態

private:
	eSceneType next_scene;       // フェード後の遷移先

public:
	TitleScene();
	virtual ~TitleScene();

	virtual void Initialize() override;
	virtual eSceneType Update(float delta_second) override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowSceneType() const override;
};
