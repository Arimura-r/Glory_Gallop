#include "SelectedBetTicketList.h"
#include "BetManager.h"
#include "TicketManager.h"
#include <assert.h>

namespace
{
	const int FONT_SIZE = 24;
	const int VERTICAL_INTERVAL = 30;
	const int HEIGHT_MAX = 960;
	const int BACK_BOX_ALPHA = 128;
	const int BACK_BOX_WIDTH = 300;
}

SelectedBetTicketListUI::SelectedBetTicketListUI(VECTOR2 _position, BetManager* _betManager, TicketManager* _ticket):UIBase(_position),
betManager(_betManager),ticketManager(_ticket)
{
}

SelectedBetTicketListUI::~SelectedBetTicketListUI()
{
}

void SelectedBetTicketListUI::DrawUI()
{
	if (not betManager->GetIsTicketContentDecide())
		return;

	//ToDo　一度に表示できる数に限界を設けて溢れた場合は複数ページに表示できるようにする

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BACK_BOX_ALPHA);
	DrawBox((int)position.x, (int)position.y, (int)(position.x + BACK_BOX_WIDTH), (int)(position.y + betManager->GetSelectedTicketsData().size() * VERTICAL_INTERVAL), 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	//カーソルが置かれている要素があるなら表示(購入ボタンにカーソルが行っている時にはカーソルを表示しない)
	int cursorY = betManager->GetTicketPriceCursorSelectedNum();
	if (cursorY > -1)
	{
		int cursorDrawY = cursorY * VERTICAL_INTERVAL + (int)position.y;
		//カーソルを選択中なら背景を赤くする
		if (betManager->GetIsTicketPriceSelect())
		{
			DrawBox((int)position.x, cursorDrawY, (int)(position.x + BACK_BOX_WIDTH), cursorDrawY + VERTICAL_INTERVAL, 0xff0000, true);
		}

		DrawBox((int)position.x, cursorDrawY, (int)(position.x + BACK_BOX_WIDTH), cursorDrawY + VERTICAL_INTERVAL, 0xffffff, false);
	}

	int ticketNum = 0;
	SetFontSize(FONT_SIZE);
	for (const auto& t : betManager->GetSelectedTicketsData())
	{
		std::string ticketType = TicketDefine::TicketTypeConvString(t.type);//馬券の種類
		std::string ticketContent=t.TicketContentToString();//馬券の中身
		std::string ticketPrice=std::to_string(t.price)+"円";//馬券の金額

		DrawFormatStringF(position.x, position.y + ticketNum * VERTICAL_INTERVAL, 0xffffff, "%3s　%10s　%s", ticketType.c_str(), ticketContent.c_str(), ticketPrice.c_str());
		ticketNum++;
	}
	SetFontSize(DEFAULT_FONT_SIZE);
}
