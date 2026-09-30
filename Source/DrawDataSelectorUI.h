#pragma once
#include "UIBase.h"
#include <map>

class GameManager;
class HorseManager;

//表示情報を変更するためのUI
class DrawDataSelectorUI :public UIBase
{
public:
	DrawDataSelectorUI(VECTOR2 _position,GameManager* _gameManager,HorseManager* _horseManager);
	~DrawDataSelectorUI();
	void Update()override;
	void DrawUI()override;

private:
	GameManager* gameManager;
	HorseManager* horseManager;
	std::map<int,int> selectImageHandle;//各選択項目用の画像
};