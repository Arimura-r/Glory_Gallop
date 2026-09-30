#pragma once
#include "UIBase.h"

class GameManager;

class TitleBackTextUI :public UIBase
{
public:
	TitleBackTextUI(VECTOR2 _position,GameManager*_gameManager);
	~TitleBackTextUI();
	void Update()override;
	void DrawUI()override;

private:
	GameManager* gameManager;
	int hImage;
	float alpha;
	float alphaChangeSpeed;
};