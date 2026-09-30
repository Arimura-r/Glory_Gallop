#pragma once
#include "UIBase.h"

class TicketManager;
class GameManager;

//Š‹à‚ğ•\¦‚·‚éUI
class HaveMoneyUI :public UIBase
{
public:
	HaveMoneyUI(VECTOR2 _position,TicketManager* _ticket,GameManager* _gameManager,bool _active=true);
	~HaveMoneyUI();

	void DrawUI()override;

private:
	TicketManager* ticketManager;
	GameManager* gameManager;
};