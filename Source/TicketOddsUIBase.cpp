#include "TicketOddsUIBase.h"
#include "BetManager.h"
#include "TicketManager.h"

const int TicketOddsUIBase::FONT_SIZE = 28;
const int TicketOddsUIBase::HORIZONTAL_INTERVAL = 150;
const int TicketOddsUIBase::VERTICAL_INTERVAL = 50;
const int TicketOddsUIBase::BACK_BOX_ALPHA = 128;

TicketOddsUIBase::TicketOddsUIBase(VECTOR2 _position, BetManager* _betManager,TicketManager* _ticketManager, int _horseNum, BetManagerDefine::BetCategory _activeCategory,TicketDefine::TicketType _ticketType) :UIBase(_position),
betManager(_betManager), ticketManager(_ticketManager), horseNum(_horseNum), activeCategory(_activeCategory),ticketType(_ticketType)
{
}

TicketOddsUIBase::~TicketOddsUIBase()
{
}

void TicketOddsUIBase::DrawUI()
{
	if (betManager->GetCurrentSelectedBetCategory() != activeCategory || betManager->GetIsTicketContentDecide())
		return;

	//背景の黒四角の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BACK_BOX_ALPHA);
	DrawBox((int)position.x, (int)position.y, (int)(position.x + (((horseNum - 1) % betManager->TICKET_UI_LINE_HORSE_NUM) + 1) * HORIZONTAL_INTERVAL),
		(int)(position.y + (((horseNum - 1) / betManager->TICKET_UI_LINE_HORSE_NUM) + 1) * VERTICAL_INTERVAL), 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	//馬番選択画面ならカーソルを表示する
	int cursorSelectNum = -1;
	if (betManager->GetIsTicketTypeSelected())
		cursorSelectNum = betManager->GetCursorTicketHorseNumber();

	SetFontSize(FONT_SIZE);
	for (int i = 0; i < horseNum; i++)
	{
		float drawX = (i % betManager->TICKET_UI_LINE_HORSE_NUM) * HORIZONTAL_INTERVAL + position.x;
		float drawY = (i / betManager->TICKET_UI_LINE_HORSE_NUM) * VERTICAL_INTERVAL + position.y;

		TicketDefine::TicketData t = TicketDefine::TicketData(-1, ticketType, i+1);
		float odds = ticketManager->GetOdds(t);//オッズ(仮表記)

		//券種が指定されていたら描画を行う
		if (cursorSelectNum > -1)
			SelectedDraw((int)drawX, (int)drawY, i, cursorSelectNum);

		DrawFormatStringF(drawX, drawY, 0xffffff, "%2d %.1f", i + 1, odds);
	}
	SetFontSize(DEFAULT_FONT_SIZE);
}

void TicketOddsUIBase::SelectedDraw(int _drawX, int _drawY, int _horseNumber, int _cursorNum)
{
	//馬券選択に選ばれているかを調べる
	bool selected = false;
	for (const auto& n : betManager->GetSelectedHorseNumbers())
	{
		if (_horseNumber == n)
		{
			selected = true;
			break;
		}
	}

	if (selected)//選択されていたら背景を赤くする
		DrawBox((int)_drawX, (int)_drawY, (int)_drawX + HORIZONTAL_INTERVAL, (int)_drawY + VERTICAL_INTERVAL, 0xff0000, true);
	//カーソルの位置には白四角を表示する
	if (_horseNumber == _cursorNum)
		DrawBox((int)_drawX, (int)_drawY, (int)_drawX + HORIZONTAL_INTERVAL, (int)_drawY + VERTICAL_INTERVAL, 0xffffff, false);
}
