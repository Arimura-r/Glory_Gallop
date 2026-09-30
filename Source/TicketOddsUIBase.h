#pragma once
#include "UIBase.h"
#include "BetManagerDefine.h"
#include "TicketDefine.h"

class BetManager;
class TicketManager;

//各馬券の券種ごとに購入を選択できる馬券とオッズを表示するクラスの基底クラス
class TicketOddsUIBase :public UIBase
{
public:
	TicketOddsUIBase(VECTOR2 _position,BetManager* _betManager,TicketManager* _ticket,int _horseNum, BetManagerDefine::BetCategory _activeCategory, TicketDefine::TicketType _ticketType);
	virtual ~TicketOddsUIBase();

	virtual void DrawUI()override;

protected:
	static const int FONT_SIZE;
	static const int HORIZONTAL_INTERVAL;
	static const int VERTICAL_INTERVAL;
	static const int BACK_BOX_ALPHA;

	//選択されているもの、カーソルが合っているものに追加で描画を行う
	virtual void SelectedDraw(int _drawX, int _drawY, int _horseNumber, int _cursorNum);

	int horseNum;
	BetManager* betManager;
	TicketManager* ticketManager;
	BetManagerDefine::BetCategory activeCategory;//どのカテゴリが選ばれた時に表示を行うか
	TicketDefine::TicketType ticketType;//馬券の種類
};