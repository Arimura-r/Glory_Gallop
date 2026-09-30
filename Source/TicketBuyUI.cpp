#include "TicketBuyUI.h"
#include "BetManager.h"

namespace
{
	const VECTOR2 IMAGE_SIZE = VECTOR2(180, 120);
}

TicketBuyUI::TicketBuyUI(VECTOR2 _position, BetManager* _betManager):UIBase(_position),betManager(_betManager)
{
	buyImage = LoadGraph("data/texture/ticketBuyText.png");
}

TicketBuyUI::~TicketBuyUI()
{
	DeleteGraph(buyImage);
}

void TicketBuyUI::DrawUI()
{
	if (not betManager->GetIsTicketContentDecide())
		return;

	DrawGraphF(position.x, position.y, buyImage, true);

	if(betManager->GetIsTicketBuySelected())
		DrawBox((int)position.x, (int)position.y, (int)(position.x + IMAGE_SIZE.x), (int)(position.y + IMAGE_SIZE.y), 0xffffff, false);
}
