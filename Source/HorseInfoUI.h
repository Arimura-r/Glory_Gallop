#pragma once
#include "UIBase.h"
#include <vector>

class HorseManager;
class GameManager;
class TicketManager;

class HorseInfoUI :public UIBase
{
public:
	HorseInfoUI(VECTOR2 _position, HorseManager* _horseManager, GameManager* _gameManager, TicketManager* _ticketManager);
	~HorseInfoUI();
	void DrawUI()override;

private:
	//過去レースの結果をcsvから読み込む
	void BeforeRaceResultLoad();
	//人気表示
	void FavoriteDraw();
	//脚質表示
	void RunningStyleDraw();
	//過去レースの結果を表示する
	void BeforeRaceResultDraw();
	//各馬についてのコメントを表示する
	void HorseCommentDraw();

	HorseManager* horseManager;
	GameManager* gameManager;
	TicketManager* ticketManager;

	std::vector<std::vector<int>>beforeRaceResults;//過去レースの着順
};