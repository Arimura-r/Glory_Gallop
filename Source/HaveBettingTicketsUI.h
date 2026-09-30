#pragma once
#include "UIBase.h"

class BetManager;
class TicketManager;

//w“ü‚µ‚½”nŒ”‚Ìˆê——•\¦‚ğ‚·‚éUI
class HaveBettingTicketsUI :public UIBase
{
public:
	HaveBettingTicketsUI(VECTOR2 _position, BetManager* _betManager,TicketManager* _ticketManager);
	~HaveBettingTicketsUI();
	void DrawUI()override;

private:
	BetManager* betManager;
	TicketManager* ticketManager;
};