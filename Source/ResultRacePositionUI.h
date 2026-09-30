#pragma once
#include "UIBase.h"

class GameManager;

//プレイヤーのゴール後の着順表示UI
class ResultRacePositionUI :public UIBase
{
public:
	ResultRacePositionUI(VECTOR2 _position,Horse* _target,GameManager* _gameManager,int _horseNum);
	~ResultRacePositionUI();
	void DrawUI()override;

private:
	GameManager* gameManager;

	int numImage;
	int racePositionImage;
	int horseNum;
};