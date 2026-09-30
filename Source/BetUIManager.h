#pragma once
#include "UIBase.h"
#include <vector>

class GameManager;
class BetManager;
class HorseManager;
class TicketManager;

//”nŒ”w“ü‚ÌUIˆ—‚ğ‚Ü‚Æ‚ß‚éƒNƒ‰ƒX
class BetUIManager :public UIBase
{
public:
	BetUIManager(VECTOR2 _position, GameManager* _gameManager,HorseManager* _horseManager,TicketManager* _ticket);
	~BetUIManager();
	void DrawUI()override;

private:
	GameManager* gameManager;
	BetManager* betManager;

	std::vector<UIBase*> betUIList;
};