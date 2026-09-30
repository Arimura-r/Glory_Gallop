#include "HaveMoneyUI.h"
#include "TicketManager.h"
#include "GameManager.h"
#include "ResultState.h"

namespace
{
	const int FONT_SIZE = 40;
	const int BACK_BOX_ALPHA = 128;
	const VECTOR2 BACK_BOX_SIZE = VECTOR2(600, 100);
}

HaveMoneyUI::HaveMoneyUI(VECTOR2 _position, TicketManager* _ticket, GameManager* _gameManager, bool _active):UIBase(_position),ticketManager(_ticket),gameManager(_gameManager)
{
	active = _active;
}

HaveMoneyUI::~HaveMoneyUI()
{
}

void HaveMoneyUI::DrawUI()
{
	//非活性状態ならリザルトの状態を確認する
	if (not active)
	{
		active = gameManager->GetResultState()->GetCurrentDrawState() == ResultState::TICKET_HIT;
		return;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BACK_BOX_ALPHA);
	DrawBox((int)position.x, (int)position.y, (int)(position.x + BACK_BOX_SIZE.x), (int)(position.y + BACK_BOX_SIZE.y), 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	SetFontSize(FONT_SIZE);
	DrawFormatStringF(position.x, position.y, 0xffffff, "所持金：%d円", ticketManager->GetMoney());
	SetFontSize(DEFAULT_FONT_SIZE);
}
