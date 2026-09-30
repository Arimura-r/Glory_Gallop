#include "HitTicketUI.h"
#include "GameManager.h"
#include "ResultState.h"
#include "TicketManager.h"

namespace
{
	const int BACK_BOX_ALPHA = 128;
	const VECTOR2 BACK_BOX_SIZE = VECTOR2(1300, 800);

	const int HIT_TICKET_TEXT_HEIGHT = 40;
	const int HIT_TICKET_FONT_SIZE = 32;
	const int NO_HIT_FONT_SIZE = 48;
}

HitTicketUI::HitTicketUI(VECTOR2 _position, GameManager* _gameManager, TicketManager* _ticketManager):UIBase(_position),
	gameManager(_gameManager),ticketManager(_ticketManager)
{
}

HitTicketUI::~HitTicketUI()
{
}

void HitTicketUI::DrawUI()
{
	if (gameManager->GetResultState()->GetCurrentDrawState() != ResultState::TICKET_HIT)
		return;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BACK_BOX_ALPHA);
	//テキストの背景に黒四角を置く
	DrawBox((int)position.x, (int)position.y, (int)(position.x + BACK_BOX_SIZE.x), (int)(position.y + BACK_BOX_SIZE.y), 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	auto& hitTickets = ticketManager->GetHitTicket();

	//当たった馬券があるかどうか
	if (hitTickets.size() > 0)
	{
		float drawY = position.y;

		SetFontSize(HIT_TICKET_FONT_SIZE);
		for (auto& t : hitTickets)
		{
			DrawFormatStringF(position.x, drawY, 0xffffff, "%6s　%12s　%5d円　→　%5d円", TicketDefine::TicketTypeConvString(t.type).c_str(), t.TicketContentToString().c_str(), t.price,ticketManager->GetTicketPayOut(t));

			drawY += HIT_TICKET_TEXT_HEIGHT;
		}
	}
	else
	{
		SetFontSize(NO_HIT_FONT_SIZE);
		DrawFormatStringF(position.x, position.y+BACK_BOX_SIZE.y/2, 0xffffff, "的中馬券なし");
	}
	SetFontSize(DEFAULT_FONT_SIZE);
	//DrawFormatStringF(position.x, position.y, 0x000000, "当たり馬券表示くらす");
}
