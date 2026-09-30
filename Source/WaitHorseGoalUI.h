#pragma once
#include "UIBase.h"

class GameManager;
class HorseManager;

//‘S”n‚ªƒS[ƒ‹‚·‚é‚Ü‚Å‘Ò‚ÂUI
class WaitHorseGoalUI :public UIBase
{
public:
	WaitHorseGoalUI(VECTOR2 _position,HorseManager* _horseManager,GameManager* _gameManager);
	~WaitHorseGoalUI();
	void Update()override;
	void DrawUI()override;
private:
	HorseManager* horseManager;
	GameManager* gameManager;
};