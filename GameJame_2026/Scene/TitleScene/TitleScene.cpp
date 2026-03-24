#include "../TitleScene/TitleScene.h"
#include "../../Fade/Fade.h"
#include "../../InputControl/InputControl.h"
#include "DxLib.h"

//--------------------------------------------
// コンストラクタ
//--------------------------------------------
TitleScene::TitleScene()
	: background_image(-1)
	, cursor_image{}
	, menu_cursor(0)
	, title_bgm(-1)
	, cursor_move_se(-1)
	, cursor_push_se(-1)
	, stick_up_old(false)
	, stick_down_old(false)
	, enter_old(false)
	, next_scene(eSceneType::eTitle)
	, title_img{}
	, resource{}
	, menu_img{}
	,title_back_img{}
{
}

TitleScene::~TitleScene()
{
}

//--------------------------------------------
// 初期化処理
//--------------------------------------------
void TitleScene::Initialize()
{
	fade = new Fade();
	fade->Initialize(true);   // フェードイン開始

	resource = ResourceManager::GetInstance();

	menu_cursor = 0;
	next_scene = eSceneType::eTitle;

	background_image = LoadGraph("Resource/image/title.png");
	//cursor_image = LoadGraph("Resource/image/cursor.png");
	cursor_image = resource->GetImages("Resource/images/triangle_img.png",1,0,0,0,0)[0];

	//title_bgm = LoadSoundMem("Resource/sounds/titleBGM.mp3");
	title_bgm = resource->GetSounds("Resource/sounds/BGM/title_BGM.mp3");
	//cursor_move_se = LoadSoundMem("Resource/sounds/cursol_move.mp3");
	cursor_move_se = resource->GetSounds("Resource/sounds/SE/cursor.mp3");
	//cursor_push_se = LoadSoundMem("Resource/sounds/cursol_push.mp3");
	cursor_push_se = resource->GetSounds("Resource/sounds/SE/cursor_push.mp3");

	title_img = resource->GetImages("Resource/images/title_img_2.png",1,0,0,0,0)[0];
	title_back_img = LoadGraph("Resource/images/title_back_img.png");

	menu_img[START] = resource->GetImages("Resource/images/start_img.png", 1,0,0,0,0)[0];
	menu_img[HELP] = resource->GetImages("Resource/images/help_img.png", 1,0,0,0,0)[0];
	menu_img[END] = resource->GetImages("Resource/images/end_img.png", 1,0,0,0,0)[0];


	ChangeNextPlayVolumeSoundMem(110, title_bgm);
	PlaySoundMem(title_bgm, DX_PLAYTYPE_LOOP);
}



//--------------------------------------------
// 更新処理
//--------------------------------------------
eSceneType TitleScene::Update(float delta_second)
{
	InputControl* pad = InputControl::GetInstance();

	// フェードアウト中処理
	if (next_scene != eSceneType::eTitle)
	{
		fade->Update();

		if (fade->GetEndFlag())
			return next_scene;

		return eSceneType::eTitle;
	}

	// ===============================
	// 上入力
	// ===============================
	if (pad->GetButtonInputState(XINPUT_BUTTON_DPAD_UP) == ePadInputState::ePress
		/*|| stick_up_press*/)
	{
		menu_cursor--;
		if (menu_cursor < 0)
			menu_cursor = 2;

		PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK);
	}

	// ===============================
	// 下入力
	// ===============================
	if (pad->GetButtonInputState(XINPUT_BUTTON_DPAD_DOWN) == ePadInputState::ePress
		/*|| stick_down_press*/)
	{
		menu_cursor++;

		if (menu_cursor > 2)
			menu_cursor = 0;

		PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK);
	}

	// ===============================
	// Enter瞬間入力化
	// ===============================
	bool enter_now = CheckHitKey(KEY_INPUT_RETURN);
	bool enter_press = (enter_now && !enter_old);
	enter_old = enter_now;

	// ===============================
	// 決定処理
	// ===============================
	if (pad->GetButtonInputState(XINPUT_BUTTON_A) == ePadInputState::ePress
		|| enter_press)
	{
		ChangeNextPlayVolumeSoundMem(180, cursor_push_se);
		PlaySoundMem(cursor_push_se, DX_PLAYTYPE_BACK);
		StopSoundMem(title_bgm);

		fade->Initialize(false);

		switch (menu_cursor)
		{
		case 0: next_scene = eSceneType::eInGame; break;
		case 1: next_scene = eSceneType::eTutorial; break;
		case 2: next_scene = eSceneType::eEnd; break;
		}
	}

	fade->Update();
	return eSceneType::eTitle;
}

//--------------------------------------------
// 描画処理
//--------------------------------------------
void TitleScene::Draw() const
{
	DrawExtendGraph(0, 0, 1280, 720, background_image, FALSE);

	unsigned int white = GetColor(255, 255, 255);

	//DrawFormatString(20, 100, white, "現在のカーソル番号 : %d", menu_cursor);

	//背景画像
	DrawGraph(0, 0, title_back_img, TRUE);

	// カーソル位置
	int cursor_y = 340 + menu_cursor * 100;
	DrawRotaGraph(450, cursor_y, 0.2, PI/2, cursor_image, TRUE);

	// GameTitleの表示
	DrawRotaGraphF(670.0, 180.0, 1.5, 0.0, title_img, TRUE);
	// 各種button画像の表示
	DrawRotaGraphF(650.0, 350.0, 1.0, 0.0, menu_img[START], TRUE);
	DrawRotaGraphF(650.0, 450.0, 1.0, 0.0, menu_img[HELP], TRUE);
	DrawRotaGraphF(650.0, 550.0, 1.0, 0.0, menu_img[END], TRUE);

	fade->Draw();
}

//--------------------------------------------
// 終了処理
//--------------------------------------------
void TitleScene::Finalize()
{
	DeleteGraph(background_image);
	//DeleteGraph(cursor_image);

	//リザルト画面に行った後に音源が流れなくなったので消してます
	//DeleteSoundMem(title_bgm);

	delete fade;
}

//--------------------------------------------
eSceneType TitleScene::GetNowSceneType() const
{
	return eSceneType::eTitle;
}
