#pragma once
#include "../Library/GameObject.h"
#include "TicketDefine.h"
#include <vector>
#include <assert.h>

class HorseManager;
class CommonManager;

class OddsManager;

//馬券の管理をするクラス
class TicketManager :public GameObject
{
public:
	TicketManager(HorseManager* _horse,CommonManager* _common);
	~TicketManager();
	void Update()override;

	/// <summary>
	/// 馬券を購入する
	/// </summary>
	/// <returns>正しく購入出来たなら1,できていないなら-1を返す</returns>
	int BetTicket(const TicketDefine::TicketData& _data);

	//レース後、馬券の払い戻しを行う
	void PayOut();

	int GetMoney()const { return money; }
	const std::vector<TicketDefine::TicketData>& GetBetTicket()const { return betTickets; }
	const std::vector<TicketDefine::TicketData>& GetHitTicket()const { return hitTickets; }

	//引数で渡された馬券の払い戻し金額を得る
	int GetTicketPayOut(const TicketDefine::TicketData& _ticket)const;

	//レース結果を馬券の形で返す
	TicketDefine::TicketData GetResultTicket(TicketDefine::TicketType _ticketType)const;

	//渡された馬券のオッズを得る
	float GetOdds(const TicketDefine::TicketData& _ticket)const;
	//引数で渡された番号の馬の人気順を得る
	int GetFavoriteOrder(int _horseNumber)const;

	const int BET_MIN_MONEY = 100;//1馬券の単位(賭けられる最低金額)

private:
	//馬券の内容が正しければtrue、異常な値があれば-1を返す
	bool ValidateBetTicket(const TicketDefine::TicketData& _data);
	//馬券が当たった時に呼ぶ関数
	void Winning(const TicketDefine::TicketData& _data);
	std::vector<TicketDefine::TicketData> betTickets;//買った馬券
	std::vector<TicketDefine::TicketData> hitTickets;//当たった馬券
	int money;//所持金

	//単勝が当たったかを確認する
	bool IsWinBetWinning(const TicketDefine::TicketData& _ticket,int _first,int _second,int _third)const;
	//複勝が当たったかを確認する
	bool IsPlaceBetWinning(const TicketDefine::TicketData& _ticket, int _first, int _second, int _third)const;
	//馬連が当たったかを確認する
	bool IsQuinellaBetWinning(const TicketDefine::TicketData& _ticket, int _first, int _second, int _third)const;
	//馬単が当たったかを確認する
	bool IsExactaBetWinning(const TicketDefine::TicketData& _ticket, int _first, int _second, int _third)const;
	//ワイドが当たったかを確認する
	bool IsQuinellaPlaceBetWinning(const TicketDefine::TicketData& _ticket, int _first, int _second, int _third)const;
	//三連複が当たったかを確認する
	bool IsTrioBetWinning(const TicketDefine::TicketData& _ticket, int _first, int _second, int _third)const;
	//三連単が当たったかを確認する
	bool IsTrifectaBetWinning(const TicketDefine::TicketData& _ticket, int _first, int _second, int _third)const;

	HorseManager* horseManager;
	CommonManager* commonManager;

	OddsManager* oddsManager;
};