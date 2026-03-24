#pragma once

#include "../../Vector2D/Vector2D.h"
#include "../../Object/GameObject/GameObject.h"
#include <vector>

// シーンの種類
enum class eSceneType
{
	eTitle,					// タイトル画面
	eTutorial,				// チュートリアル画面
	eTutorialCharacter,		// キャラクターチュートリアル画面
	eTutorialEnemy,			// エネミーチュートリアル画面
	eTutorialCompany,		// 会社概要画面
	eInGame,				// インゲーム画面
	eClear,				    // 防衛成功画面
	eMiss,                  // 防衛失敗画面
	eEnd,					// エンド画面
};

// 基底シーンクラス
// インターフェースクラス
class SceneBase
{
protected:
	// ゲームオブジェクトの動的配列
	std::vector<GameObject*> objects;
	int background_image;

public:
	SceneBase();
	virtual ~SceneBase();

public:
	virtual void Initialize();
	virtual eSceneType Update(float delta_second);
	virtual void Draw() const;
	virtual void Finalize();

public:
	// 純粋仮想関数
	// 現在のシーンを返す
	virtual eSceneType GetNowSceneType() const = 0;

protected:
	// ゲームオブジェクトの生成
	template <class T>
	T* CreateObject(const Vector2D& location)
	{
		T* new_instance = new T();

		// new_instanceがGameObjectを継承しているのかチェック
		GameObject* new_object = dynamic_cast<GameObject*>(new_instance);

		// キャストに失敗したら
		if (new_object == nullptr)
		{
			delete new_object;
			throw("ゲームオブジェクトが生成できませんでした\n");
		}

		// ゲームオブジェクトの生成時座標を設定
		new_object->SetLocation(location);

		// ゲームオブジェクトの初期化
		new_object->Initialize();

		// objectsの末尾にゲームオブジェクトを追加
		objects.push_back(new_object);

		// インスタンスを返却
		return new_instance;
	}

	void DestroyObject(GameObject* target);
};