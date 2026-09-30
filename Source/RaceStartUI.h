#pragma once
#include "UIBase.h"

class GameManager;

//ボタンを押したらスタートを表示するクラス
class RaceStartUI :public UIBase
{
public:
	RaceStartUI(VECTOR2 _position,GameManager* _gameManager);
	~RaceStartUI();
	void DrawUI()override;

private:
	GameManager* gameManager;
	int startImage;
};