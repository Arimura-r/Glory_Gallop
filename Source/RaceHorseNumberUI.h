#pragma once
#include "UIBase.h"

class HorseManager;
class GameManager;
class Camera;

//レース中の馬の頭上に番号や名前を表示するクラス
class RaceHorseNumberUI :public UIBase
{
public:
	RaceHorseNumberUI(HorseManager* _horseManager, GameManager* _gameManager,Camera* _camera);
	~RaceHorseNumberUI();

	void DrawUI()override;

private:
	HorseManager* horseManager;
	GameManager* gameManager;
	Camera* camera;
};