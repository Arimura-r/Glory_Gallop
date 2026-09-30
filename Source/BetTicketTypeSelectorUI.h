#pragma once
#include "UIBase.h"
#include <map>

class BetManager;

//選んでいる馬券の券種を表示するクラス
class BetTicketTypeSelectorUI :public UIBase
{
public:
	BetTicketTypeSelectorUI(VECTOR2 _position, BetManager* _betManager);
	~BetTicketTypeSelectorUI();
	void DrawUI()override;

private:
	std::map<int, int> ticketImageHandle;//各選択項目用の画像
	BetManager* betManager;
};