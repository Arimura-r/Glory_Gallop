#include "PayOutResultUI.h"
#include "GameManager.h"
#include "ResultState.h"
#include "HorseManager.h"
#include "Horse.h"
#include "TicketManager.h"

namespace
{
	const int FONT_SIZE = 32;
	const int TEXT_HEIGHT = 40;
	const int HORSE_POSITION_BACK_BOX_WIDTH = 660;
	const VECTOR2 RACE_POSITION_OFFSET = VECTOR2(200, 0);
	const int BACK_BOX_ALPHA = 128;

	const VECTOR2 PAY_OUT_OFFSET = VECTOR2(0, 300);
	const VECTOR2 PAY_OUT_BACK_BOX_SIZE = VECTOR2(1200, 250);
	const int PAY_OUT_TEXT_WIDTH = 650;
}

PayOutResultUI::PayOutResultUI(VECTOR2 _position, GameManager* _gameManager, HorseManager* _horse, TicketManager* _ticketManager):UIBase(_position),gameManager(_gameManager),horseManager(_horse),ticketManager(_ticketManager)
{
}

PayOutResultUI::~PayOutResultUI()
{
}

void PayOutResultUI::DrawUI()
{
	if (gameManager->GetResultState()->GetCurrentDrawState() != ResultState::PAY_OUT_RESULT)
		return;

	SetFontSize(FONT_SIZE);

	DrawRacePosition();
	DrawPayOutPrice();

	SetFontSize(DEFAULT_FONT_SIZE);
}

void PayOutResultUI::DrawRacePosition()
{
	int drawX = (int)(position.x + RACE_POSITION_OFFSET.x);
	int drawY = (int)(position.y + RACE_POSITION_OFFSET.y);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BACK_BOX_ALPHA);
	//テキストの背景に黒四角を置く
	DrawBox(drawX, drawY, drawX + HORSE_POSITION_BACK_BOX_WIDTH, drawY + 3 * TEXT_HEIGHT, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	for (int i = 0; i < 3; i++)
	{
		const Horse* h = horseManager->GetHorse(i);

		int horseNumber = h->GetHorseNumber();
		int horseFavoriteOrder = ticketManager->GetFavoriteOrder(h->GetHorseNumber());
		DrawFormatString(drawX, drawY + TEXT_HEIGHT * i, 0xffffff, "%d着　　%2d　%18s　%2d人気", i + 1, horseNumber, h->GetParam().name.c_str(), horseFavoriteOrder);
	}
}

void PayOutResultUI::DrawPayOutPrice()
{
	VECTOR2 drawPos = VECTOR2(position.x + PAY_OUT_OFFSET.x, position.y + PAY_OUT_OFFSET.y);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BACK_BOX_ALPHA);
	//テキストの背景に黒四角を置く
	DrawBox((int)drawPos.x, (int)drawPos.y, (int)(drawPos.x + PAY_OUT_BACK_BOX_SIZE.x), (int)(drawPos.y + PAY_OUT_BACK_BOX_SIZE.y), 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	for(int i=0;i<TicketDefine::TYPE_NUM;i++)
	{
		//ワイドより後の馬券は改行する
		if ((TicketDefine::TicketType)i == TicketDefine::QUINELLA_PLACE)
		{
			drawPos.x += PAY_OUT_TEXT_WIDTH;
			drawPos.y = position.y + PAY_OUT_OFFSET.y;
		}
		int lineNum = DrawPayOut(drawPos, ticketManager->GetResultTicket((TicketDefine::TicketType)i));
		drawPos.y += lineNum * TEXT_HEIGHT;
	}
}

int PayOutResultUI::DrawPayOut(VECTOR2 _position, TicketDefine::TicketData _ticket)
{
	//当たり馬券が複数ある複勝、ワイドは処理を分ける
	if (_ticket.type == TicketDefine::PLACE)
	{
		int payOutPrice = ticketManager->BET_MIN_MONEY * (int)(ticketManager->GetOdds(_ticket) * 10) / 10;//オッズを元にした返金額
		int lineNum = 1;

		DrawFormatStringF(_position.x, _position.y, 0xffffff, "%6s　%12s　%5d円", TicketDefine::TicketTypeConvString(_ticket.type).c_str(), _ticket.TicketContentToString().c_str(), payOutPrice);

		_ticket.firstHorse = _ticket.secondHorse;//2着馬の複勝オッズの表示
		payOutPrice = ticketManager->BET_MIN_MONEY * (int)(ticketManager->GetOdds(_ticket) * 10) / 10;//オッズを元にした返金額
		DrawFormatStringF(_position.x, _position.y + TEXT_HEIGHT * lineNum, 0xffffff, "%6s　%12s　%5d円", TicketDefine::TicketTypeConvString(_ticket.type).c_str(), _ticket.TicketContentToString().c_str(), payOutPrice);
		lineNum++;

		//頭数が8頭以上なら3着まで複勝を当てはめる
		if (horseManager->GetHorses().size() > 7)
		{
			_ticket.firstHorse = _ticket.thirdHorse;//3着馬の複勝オッズの表示
			payOutPrice = ticketManager->BET_MIN_MONEY * (int)(ticketManager->GetOdds(_ticket) * 10) / 10;//オッズを元にした返金額
			DrawFormatStringF(_position.x, _position.y +TEXT_HEIGHT*lineNum, 0xffffff, "%6s　%12s　%5d円", TicketDefine::TicketTypeConvString(_ticket.type).c_str(), _ticket.TicketContentToString().c_str(), payOutPrice);
			lineNum++;
		}

		return lineNum;
	}
	else if (_ticket.type == TicketDefine::QUINELLA_PLACE)
	{
		int payOutPrice = (int)(ticketManager->BET_MIN_MONEY * 10.0f);//オッズが存在しないため仮の表示
		int lineNum = 1;
		int second = _ticket.secondHorse;

		//1,2着馬のワイドオッズの表示
		DrawFormatStringF(_position.x, _position.y, 0xffffff, "%6s　%12s　%5d円", TicketDefine::TicketTypeConvString(_ticket.type).c_str(), _ticket.TicketContentToString().c_str(), payOutPrice);

		_ticket.secondHorse = _ticket.thirdHorse;//1,3着馬のワイドオッズの表示
		DrawFormatStringF(_position.x, _position.y + TEXT_HEIGHT * lineNum, 0xffffff, "%6s　%12s　%5d円", TicketDefine::TicketTypeConvString(_ticket.type).c_str(), _ticket.TicketContentToString().c_str(), payOutPrice);
		lineNum++;

		_ticket.firstHorse = second;//2,3着馬のワイドオッズの表示
		DrawFormatStringF(_position.x, _position.y + TEXT_HEIGHT * lineNum, 0xffffff, "%6s　%12s　%5d円", TicketDefine::TicketTypeConvString(_ticket.type).c_str(), _ticket.TicketContentToString().c_str(), payOutPrice);
		lineNum++;

		return lineNum;
	}
	else
	{
		int payOutPrice = (int)(ticketManager->BET_MIN_MONEY * 10.0f);//オッズが存在しないため仮の表示
		//単勝のみオッズから返金額を求める
		if (_ticket.type == TicketDefine::WIN)
			payOutPrice = ticketManager->BET_MIN_MONEY * (int)(ticketManager->GetOdds(_ticket) * 10) / 10;//オッズを元にした返金額

		DrawFormatStringF(_position.x, _position.y, 0xffffff, "%6s　%12s　%5d円", TicketDefine::TicketTypeConvString(_ticket.type).c_str(), _ticket.TicketContentToString().c_str(), payOutPrice);
		return 1;
	}
	return -1;
}
