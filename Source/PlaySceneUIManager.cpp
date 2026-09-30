#include "PlaySceneUIManager.h"
#include "HorseManager.h"

#include "RaceHorseNumberUI.h"

PlaySceneUIManager::PlaySceneUIManager(HorseManager* _horse, GameManager* _gameManager, Camera* _camera)
{
	//プレイヤーが存在しない観戦モードの時のみ生成するクラス
	if (_horse->GetPlayer() == nullptr)
	{
		uiList.push_back(new RaceHorseNumberUI(_horse, _gameManager, _camera));
	}
}

PlaySceneUIManager::~PlaySceneUIManager()
{
}

void PlaySceneUIManager::Update()
{
}

void PlaySceneUIManager::DrawUI()
{
	for (const auto& u : uiList)
	{
		u->DrawUI();
	}
}
