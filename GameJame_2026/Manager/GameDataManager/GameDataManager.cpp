#include "GameDataManager.h"
#include "DxLib.h"

GameDataManager* GameDataManager::instance = nullptr;

GameDataManager::GameDataManager()
{
}

GameDataManager* GameDataManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new GameDataManager();
	}
	return instance;
}

void GameDataManager::DeleteInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

void GameDataManager::SaveData()
{

}

void GameDataManager::LoadData()
{

}