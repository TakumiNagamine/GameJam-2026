#pragma once
#include "../../Vector2D/Vector2D.h"

class GameDataManager
{
private:
	static GameDataManager* instance;

	Vector2D position[6];	// parts座標の格納変数

public:
	GameDataManager();
	~GameDataManager() = default;

public:
	//疑似的なコンストラクタ
	static GameDataManager* GetInstance();
	//疑似的なコンストラクタ
	static void DeleteInstance();

public:

	void SaveData();

	void LoadData();
};