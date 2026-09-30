#include "BetTicketDecideUI.h"
#include "BetManager.h"

namespace
{
	VECTOR2 IMAGE_SIZE = VECTOR2(180, 120);
}

BetTicketDecideUI::BetTicketDecideUI(VECTOR2 _position, BetManager* _betManager):UIBase(_position),betManager(_betManager)
{
	textImage = LoadGraph("data/texture/ticketDecideText.png");
}

BetTicketDecideUI::~BetTicketDecideUI()
{
	DeleteGraph(textImage);
}

void BetTicketDecideUI::DrawUI()
{
	//馬券の内容が確定済みか購入馬券一覧なら表示しない
	if(betManager->GetIsTicketContentDecide()||betManager->GetCurrentSelectedBetCategory()== BetManagerDefine::BETTING_TICKET)
		return;

	DrawGraphF(position.x, position.y, textImage, true);
	//カーソルの表示
	if(betManager->GetIsSelectedTicketDecide()&&betManager->GetIsTicketTypeSelected())
		DrawBox((int)position.x, (int)position.y, (int)(position.x + IMAGE_SIZE.x), (int)(position.y + IMAGE_SIZE.y), 0xffffff, false);
}
