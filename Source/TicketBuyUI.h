#pragma once
#include "UIBase.h"

class BetManager;

//”nŒ”‚Ìw“üƒ{ƒ^ƒ“‚ÌUI
class TicketBuyUI :public UIBase
{
public:
	TicketBuyUI(VECTOR2 _position, BetManager* _betManager);
	~TicketBuyUI();
	void DrawUI()override;

private:
	BetManager* betManager;
	int buyImage;
};