#pragma once
#include "TicketOddsUIBase.h"

class TicketPlaceOddsUI :public TicketOddsUIBase
{
public:
	TicketPlaceOddsUI(VECTOR2 _position, BetManager* _betManager, TicketManager* _ticket, int _horseNum);
	~TicketPlaceOddsUI();
};