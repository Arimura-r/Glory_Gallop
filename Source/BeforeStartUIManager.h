#pragma once
#include "../Library/GameObject.h"
#include <vector>

class UIBase;
class GameManager;

class BeforeStartUIManager :public GameObject
{
public:
	BeforeStartUIManager(GameManager* _gameManager);
	~BeforeStartUIManager();

	void Update()override;
	//UI‚ð•`‰æ‚·‚é
	void DrawUI();

private:
	std::vector<UIBase*> uiList;
};