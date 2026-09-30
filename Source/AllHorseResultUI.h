#pragma once
#include "UIBase.h"

class HorseManager;
class GameManager;

//‘S”n‚Ì’…‡•\¦UI
class AllHorseResultUI :public UIBase
{
public:
	AllHorseResultUI(VECTOR2 _position,HorseManager* _horse,GameManager* _gameManager);
	~AllHorseResultUI();
	void Update()override;
	void DrawUI()override;

private:
	GameManager* gameManager;
	HorseManager* horseManager;
};