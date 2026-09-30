#pragma once
#include "UIBase.h"

class GameManager;

class StartCountUI :public UIBase
{
public:
	StartCountUI(VECTOR2 _position,GameManager* _gameManager);
	~StartCountUI();
	void DrawUI()override;

private:
	GameManager* gameManager;
	int numImage;
};