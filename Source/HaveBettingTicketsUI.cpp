#include "HaveBettingTicketsUI.h"
#include "BetManager.h"
#include "TicketManager.h"
#include "TicketDefine.h"

namespace
{
	const int FONT_SIZE = 24;
	const int VERTICAL_INTERVAL = 30;
	const int HORIZONTAL_INTERVAL = 350;
	const int HEIGHT_MAX = 960;
	const int BACK_BOX_ALPHA = 128;
	const int BACK_BOX_HEIGHT = 1000;
	const int LINE_NUM = 4;//s‚Ì”
}

HaveBettingTicketsUI::HaveBettingTicketsUI(VECTOR2 _position, BetManager* _betManager, TicketManager* _ticketManager):UIBase(_position),betManager(_betManager), ticketManager(_ticketManager)
{
}

HaveBettingTicketsUI::~HaveBettingTicketsUI()
{
}

void HaveBettingTicketsUI::DrawUI()
{
	if (betManager->GetCurrentSelectedBetCategory() != BetManagerDefine::BETTING_TICKET)
		return;

	//”wŒi‚Ì•ŽlŠp‚Ì•`‰æ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BACK_BOX_ALPHA);
	DrawBox((int)position.x, (int)position.y, (int)(position.x + Screen::WIDTH), BACK_BOX_HEIGHT, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	SetFontSize(FONT_SIZE);

	int ticketIndex = 0;
	float drawX = position.x;
	float drawY = position.y;
	int lineNum = 1;
	for (const auto& t : ticketManager->GetBetTicket())
	{ 
		std::string ticketType = TicketDefine::TicketTypeConvString(t.type);//”nŒ”‚ÌŽí—Þ
		std::string ticketContent = t.TicketContentToString();//”nŒ”‚Ì’†g
		std::string ticketPrice = std::to_string(t.price) + "‰~";//”nŒ”‚Ì‹àŠz
		DrawFormatStringF(drawX, drawY, 0xffffff, "%3s@%10s@%6s", ticketType.c_str(), ticketContent.c_str(), ticketPrice.c_str());

		drawY += VERTICAL_INTERVAL;
		//ˆê”Ô‰º‚Ü‚Ås‚Á‚½‚çŽŸ‚Ìs‚É‚¸‚ç‚·
		if (drawY > HEIGHT_MAX)
		{
			drawY = position.y;
			drawX += HORIZONTAL_INTERVAL;
			lineNum++;

			//Å‘å”‚É’B‚µ‚½‚ç‚±‚êˆÈã•`‰æ‚µ‚È‚¢@ToDoƒy[ƒW§‚É‚·‚é
			if (lineNum > LINE_NUM)
				return;
		}

		ticketIndex++;
	}
	SetFontSize(DEFAULT_FONT_SIZE);
}
