#pragma once
#include "UIBase.h"

class GameManager;

//ボタンを押したら次へ　を表示するUI
class PressNextUI :public UIBase
{
public:
	PressNextUI(VECTOR2 _position, GameManager* _gameManager);
	~PressNextUI();
	void Update()override;
	void DrawUI()override;

private:
	GameManager* gameManager;
	int hImage;
	float alpha;
	float alphaChangeSpeed;
};