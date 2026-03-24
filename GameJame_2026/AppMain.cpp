#include "DxLib.h"
#include "InputControl/InputControl.h"
#include "Manager/ResourceManager/ResourceManager.h"
#include "Manager/SceneManager/SceneManager.h"

//1フレームにかかった時間を計測して返す
static float GetDeltaSecond()
{
	//前回の計測時間
	static LONGLONG prev_time = GetNowHiPerformanceCount();
	//現在までの時間
	LONGLONG current_time = GetNowHiPerformanceCount();

	//現在から過去の時間を引く
	//μ秒→秒に変換する
	float result = (float)(current_time - prev_time) * 1.0e-6f;

	//1フレームにかかった時間を返す
	prev_time = current_time;

	return result;
}

// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	// ウィンドウタイトルの設定
	SetMainWindowText("Lチーム");

	ChangeWindowMode(TRUE);

	//画面サイズ指定
	SetGraphMode(1280, 720, 32);

	if (DxLib_Init() == -1)
	{
		return -1;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	SceneManager* manager = nullptr;
	int result = 0;

	try
	{
		// SceneManagerのオブジェクト化
		manager = new SceneManager();

		// SceneManagerの初期化処理
		manager->Initialize();

		// パッド入力制御のインスタンスを取得
		InputControl* pad_input = InputControl::GetInstance();


		// マウスカーソル非表示
		SetMouseDispFlag(FALSE);

		while (ProcessMessage() != -1 && manager->LoopCheck())
		{

			// 入力更新処理
			pad_input->Update();

			ClearDrawScreen();

			// シーン更新処理
			manager->Update(GetDeltaSecond());

			// FPS描画
			//fps.Display_FPS();

			ScreenFlip();

			if (pad_input->GetButtonInputState(XINPUT_BUTTON_BACK) == ePadInputState::eRelease)
			{
				// ESCAPEキーかBACKボタンで終了
				break;
			}
		}
	}
	catch (std::string& error_text)
	{
		OutputDebugString(error_text.c_str());
		result = -1;
	}
	// パッド入力制御の終了処理
	InputControl::DeleteInstance();
	// リソース管理の終了処理
	ResourceManager::DeleteInstance();

	if (manager != nullptr)
	{
		manager->Finalize();
		// メモリの開放
		delete manager;
	}

	DxLib_End();

	return 0;
}