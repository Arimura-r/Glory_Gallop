#include "TicketWinOddsUI.h"
#include "BetManager.h"

TicketWinOddsUI::TicketWinOddsUI(VECTOR2 _position, BetManager* _betManager, TicketManager* _ticket, int _horseNum):TicketOddsUIBase(_position,_betManager,_ticket,_horseNum,BetManagerDefine::WIN, TicketDefine::WIN)
{
}

TicketWinOddsUI::~TicketWinOddsUI()
{
}
