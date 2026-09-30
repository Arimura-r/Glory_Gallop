#pragma once
#include "UIBase.h"

class GameManager;

//レースのタイム表示用のクラス
class RaceTimeUI :public UIBase
{
public:
	RaceTimeUI(VECTOR2 _position, GameManager* _gameManager);
	~RaceTimeUI();
	void DrawUI()override;

private:
	GameManager* gameManager;

	//ToDo　描画座標などはManagerでまとめて読んで受け取る形にする
};