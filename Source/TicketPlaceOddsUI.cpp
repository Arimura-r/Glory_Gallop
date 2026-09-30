#include "TicketPlaceOddsUI.h"
#include "BetManager.h"

TicketPlaceOddsUI::TicketPlaceOddsUI(VECTOR2 _position, BetManager* _betManager, TicketManager* _ticket, int _horseNum):TicketOddsUIBase(_position,_betManager,_ticket,_horseNum,BetManagerDefine::PLACE,TicketDefine::PLACE)
{
}

TicketPlaceOddsUI::~TicketPlaceOddsUI()
{
}
