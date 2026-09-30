#include "BeforeStartUIManager.h"
#include "UIBase.h"
#include "StartCountUI.h"

BeforeStartUIManager::BeforeStartUIManager(GameManager* _gameManager)
{
	uiList.push_back(new StartCountUI(VECTOR2(Screen::WIDTH_CENTER,Screen::HEIGHT_CENTER),_gameManager));
}

BeforeStartUIManager::~BeforeStartUIManager()
{
}

void BeforeStartUIManager::Update()
{
}

void BeforeStartUIManager::DrawUI()
{
	for (const auto& u : uiList)
	{
		u->DrawUI();
	}
}