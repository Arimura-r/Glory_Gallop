#pragma once
#include "UIBase.h"

class BetManager;

class BetTicketDecideUI :public UIBase
{
public:
	BetTicketDecideUI(VECTOR2 _position,BetManager* _betManager);
	~BetTicketDecideUI();
	void DrawUI()override;

private:
	BetManager* betManager;
	int textImage;
};