#pragma once
#include "UIBase.h"

class GameManager;
class TicketManager;

//当たった馬券を表示するクラス
class HitTicketUI :public UIBase
{
public:
	HitTicketUI(VECTOR2 _position,GameManager* _gameManager,TicketManager* _ticketManager);
	~HitTicketUI();
	void DrawUI()override;

private:
	GameManager* gameManager;
	TicketManager* ticketManager;
};