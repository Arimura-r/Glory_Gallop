#pragma once
#include "UIBase.h"

class HorseManager;
class GameManager;

//全出走馬を一覧で表示するUI
class AllHorseDataUI :public UIBase
{
public:
	AllHorseDataUI(VECTOR2 _position,HorseManager* _horse,GameManager* _game);
	~AllHorseDataUI();
	void Update()override;
	void DrawUI()override;
	
private:
	HorseManager* horseManager;
	GameManager* gameManager;
};