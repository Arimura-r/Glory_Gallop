#pragma once
#include "UIBase.h"

class TicketManager;
class BetManager;

//w“ü—\’è‚Ì”nŒ”‚ğˆê——‚Å•\¦‚·‚éUI
class SelectedBetTicketListUI :public UIBase
{
public:
	SelectedBetTicketListUI(VECTOR2 _position, BetManager* _betManager, TicketManager* _ticket);
	~SelectedBetTicketListUI();
	void DrawUI()override;

private:
	BetManager* betManager;
	TicketManager* ticketManager;
};