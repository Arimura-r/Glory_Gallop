#pragma once
#include "../Library/GameObject.h"
#include "TicketDefine.h"
#include "BetManagerDefine.h"
#include <vector>

class InputManager;
class SoundManager;
class HorseManager;
class TicketManager;

//馬券購入を管理するクラス
//TicketManager側の関数を呼んで馬券購入を行う
class BetManager :public GameObject
{
public:
	BetManager(TicketManager* _ticket,HorseManager* _horse,InputManager* _input,SoundManager* _sound);
	~BetManager();
	void BetUpdate();

	//現在選択されている馬券の種類を得る
	TicketDefine::TicketType GetSelectedTicketType()const;
	BetManagerDefine::BetCategory GetCurrentSelectedBetCategory()const { return currentSelectedBetCategory; }
	bool GetIsTicketTypeSelected()const { return ticketTypeSelect; }

	//カーソルが置かれている馬券の番号
	int GetCursorTicketHorseNumber()const;
	const std::vector<int>& GetSelectedHorseNumbers()const { return selectedHorseNumbers; }

	//馬券の購入決定が選ばれているかを調べる
	bool GetIsSelectedTicketDecide()const;
	bool GetIsTicketContentDecide()const { return ticketContentDecide; }
	const std::vector<TicketDefine::TicketData>& GetSelectedTicketsData()const { return selectedBetTickets; }

	//馬券購入金額選択画面で、何番目の要素にカーソルがあるかを得る　馬券自体を選択していない場合は-1を返す
	int GetTicketPriceCursorSelectedNum()const;
	//馬券購入ボタンが選ばれているかを調べる
	bool GetIsTicketBuySelected()const;
	//馬券購入金額選択画面で、どこかの要素を選択しているかどうか
	bool GetIsTicketPriceSelect()const { return ticketPriceSelect; }

	const int TICKET_UI_LINE_HORSE_NUM = 9;//馬券購入用のUIで、一列に何頭表示するか
	const int BET_TICKETS_LIST_DRAW_LINE = 1;//購入を予定している馬券を横何列まで表示させるか
private:
	//デバッグ確認用のImGuiのUpdate
	void ImGuiUpdate();
	//馬券の種類を変更するところのUpdate
	void BetTicketTypeSelectUpdate();
	//馬券の内容(馬番)を決めるところのUpdate
	void BetHorseNumberSelectUpdate();
	//馬券の金額、購入確定を決めるところのUpdate
	void BetPriceSelectUpdate();
	//選択した馬券の金額を変える際のUpdate
	void SelectTicketPriceChangeUpdate();

	//配列に新しく選択された馬番を追加する　既にある番号なら逆に選択を解除する
	void AddTicketSelectedHorse();
	//選択されている番号を購入予定の馬券の形に変換する
	const std::vector<TicketDefine::TicketData> SelectNumToTicketData(const std::vector<int> _nums);
	//購入予定の馬券を選択されている番号の形に変換する
	const std::vector<int> TicketDataToSelectNum(const std::vector<TicketDefine::TicketData> _tickets);
	//現在の情報で馬券の購入を確定する
	void BuyTicket();
	//現在購入を予約している馬券の合計金額を得る
	int GetSelectedTicketsPriceSum()const;

	//購入馬券を決めるために必要な変数を初期化する
	void BetDataReset();

	TicketManager* ticketManager;
	HorseManager* horseManager;
	InputManager* input;
	SoundManager* sound;

	bool ticketTypeSelect;//馬券の種類を決めているか
	VECTOR2 ticketHorseNumberCursorPos;//馬券購入用の馬番決定用のカーソルの位置
	std::vector<int> selectedHorseNumbers;//選択している馬番をまとめるリスト
	bool ticketContentDecide;//馬券の内容が決定されているか
	VECTOR2 ticketPriceCursorPos;//馬券購入用の金額確定用のカーソルの位置
	bool ticketPriceSelect;//馬券購入用の金額確定時のカーソルでどこかを選択しているか

	BetManagerDefine::BetCategory currentSelectedBetCategory;
	TicketDefine::TicketData betTicket;//デバッグ確認用
	std::vector<TicketDefine::TicketData> selectedBetTickets;//購入を予定している馬券のリスト
};