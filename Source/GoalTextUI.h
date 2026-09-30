#pragma once
#include "UIBase.h"

class GameManager;

class GoalTextUI :public UIBase
{
public:
	GoalTextUI(VECTOR2 _position, GameManager* _gameManager);
	~GoalTextUI();
	void DrawUI()override;

private:
	GameManager* gameManager;
	int hImage;
	float scale;
};