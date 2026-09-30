#pragma once
#include "TicketOddsUIBase.h"

//購入できる単勝馬券とそのオッズを表示するクラス
class TicketWinOddsUI :public TicketOddsUIBase
{
public:
	TicketWinOddsUI(VECTOR2 _position, BetManager* _betManager, TicketManager* _ticket, int _horseNum);
	~TicketWinOddsUI();
};