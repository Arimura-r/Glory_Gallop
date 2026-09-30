#pragma once
#include "UIBase.h"
#include "TicketDefine.h"

class GameManager;
class HorseManager;
class TicketManager;

//各馬券の払い戻しを表示するUI
class PayOutResultUI :public UIBase
{
public:
	PayOutResultUI(VECTOR2 _position,GameManager* _gameManager,HorseManager* _horse,TicketManager* _ticketManager);
	~PayOutResultUI();
	void DrawUI()override;

private:
	//3着までの馬の着順表示
	void DrawRacePosition();
	//各馬券の払い戻し金額表示
	void DrawPayOutPrice();

	//馬券の払い戻しの描画を行う　返り値は描画に使用した行の数(基本1だが、複勝ワイドのみ複数行に分けるため2,3になる)
	int DrawPayOut(VECTOR2 _position, TicketDefine::TicketData _ticket);

	GameManager* gameManager;
	HorseManager* horseManager;
	TicketManager* ticketManager;
};