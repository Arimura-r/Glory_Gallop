#include "BetManager.h"
#include "../ImGui/imgui.h"
#include "TicketManager.h"
#include "HorseManager.h"
#include "Horse.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "TicketManager.h"

BetManager::BetManager(TicketManager* _ticket, HorseManager* _horse, InputManager* _input, SoundManager* _sound) :
	ticketManager(_ticket), horseManager(_horse), input(_input), sound(_sound), ticketTypeSelect(false), currentSelectedBetCategory(BetManagerDefine::BETTING_TICKET),
	ticketContentDecide(false),ticketPriceSelect(false)
{
}

BetManager::~BetManager()
{
}

void BetManager::BetUpdate()
{
	//馬券の種類の選択画面かどうか
	if (ticketTypeSelect)
	{
		//購入馬券の内容を確定しているか
		if (ticketContentDecide)
		{
			//カーソルを選択しているかどうか
			if (ticketPriceSelect)
			{
				//選択中の要素が馬券の場合金額の変更を行う
				if (ticketPriceCursorPos.x < BET_TICKETS_LIST_DRAW_LINE)
				{
					SelectTicketPriceChangeUpdate();
				}
				else//選択中の要素が購入ボタンの場合馬券を購入させる
				{
					//所持金が最低金額より上なら馬券を購入する
					if(ticketManager->GetMoney()>=ticketManager->BET_MIN_MONEY)
						BuyTicket();
					else
					{
						sound->Play("Cannot");
						ticketPriceSelect = false;
					}
				}
			}
			else
			{
				//選択していない場合、カーソルの移動を行う
				BetPriceSelectUpdate();
			}
		}
		else
		{
			BetHorseNumberSelectUpdate();
		}
	}
	else
	{
		//選択している馬券種類の変更
		BetTicketTypeSelectUpdate();
	}

	//ImGuiUpdate();
}

TicketDefine::TicketType BetManager::GetSelectedTicketType() const
{
	TicketDefine::TicketType ret = TicketDefine::NONE;
	switch (currentSelectedBetCategory)
	{
	case BetManagerDefine::BETTING_TICKET:
		break;
	case BetManagerDefine::WIN:
		ret = TicketDefine::WIN;
		break;
	case BetManagerDefine::PLACE:
		ret = TicketDefine::PLACE;
		break;
	case BetManagerDefine::QUINELLA:
		ret = TicketDefine::QUINELLA;
		break;
	case BetManagerDefine::EXACTA:
		ret = TicketDefine::EXACTA;
		break;
	case BetManagerDefine::QUINELLA_PLACE:
		ret = TicketDefine::QUINELLA_PLACE;
		break;
	case BetManagerDefine::TRIO:
		ret = TicketDefine::TRIO;
		break;
	case BetManagerDefine::TRIFECTA:
		ret = TicketDefine::TRIFECTA;
		break;
	default:
		assert(false && "異常な馬券カテゴリが指定されています");
		break;
	}

	return ret;
}

int BetManager::GetCursorTicketHorseNumber() const
{
	return (int)ticketHorseNumberCursorPos.y * TICKET_UI_LINE_HORSE_NUM + (int)ticketHorseNumberCursorPos.x;;
}

bool BetManager::GetIsSelectedTicketDecide() const
{
	//馬番より下にカーソルがあるなら決定が選ばれていることにする
	return ticketHorseNumberCursorPos.y >= horseManager->GetHorses().size() / TICKET_UI_LINE_HORSE_NUM;
}

int BetManager::GetTicketPriceCursorSelectedNum() const
{
	int ret = -1;
	if (ticketPriceCursorPos.x <BET_TICKETS_LIST_DRAW_LINE)
		ret = (int)ticketPriceCursorPos.y;
	return ret;
}

bool BetManager::GetIsTicketBuySelected() const
{
	return ticketPriceCursorPos.x >= BET_TICKETS_LIST_DRAW_LINE;
}

void BetManager::ImGuiUpdate()
{
	int maxHorseNum = (int)horseManager->GetHorses().size() - 1;

	ImGui::Begin("Bet");
	ImGui::SliderInt("Price", &betTicket.price, 1, ticketManager->GetMoney() / ticketManager->BET_MIN_MONEY);
	ImGui::SliderInt("TicketType", (int*)&betTicket.type, 0, TicketDefine::TYPE_NUM - 1);
	ImGui::SliderInt("FirstHorse", &betTicket.firstHorse, 0, maxHorseNum);
	if (betTicket.type != TicketDefine::WIN && betTicket.type != TicketDefine::PLACE)
	{
		ImGui::SliderInt("SecondHorse", &betTicket.secondHorse, 0, maxHorseNum);
		if (betTicket.type == TicketDefine::TRIO || betTicket.type == TicketDefine::TRIFECTA)
		{
			ImGui::SliderInt("ThirdHorse", &betTicket.thirdHorse, 0, maxHorseNum);
		}
	}
	if (ImGui::Button("Bet"))
	{
		betTicket.price *= 100;
		ticketManager->BetTicket(betTicket);
	}
	//デバッグ用　指定した券種を全通り買う
	if (ImGui::Button("AllBet"))
	{
		betTicket.price = 100;
		for (auto& i : horseManager->GetHorses())
		{
			//1列目のみの馬券
			if (betTicket.type == TicketDefine::WIN || betTicket.type == TicketDefine::PLACE)
			{
				betTicket.firstHorse = i->GetHorseNumber() - 1;
				ticketManager->BetTicket(betTicket);
				continue;
			}

			for (auto& j : horseManager->GetHorses())
			{
				//重複を削除
				if (i == j)
					continue;

				//2列目までの馬券
				if (betTicket.type == TicketDefine::QUINELLA || betTicket.type == TicketDefine::EXACTA || betTicket.type == TicketDefine::QUINELLA_PLACE)
				{
					betTicket.firstHorse = i->GetHorseNumber() - 1;
					betTicket.secondHorse = j->GetHorseNumber() - 1;
					ticketManager->BetTicket(betTicket);
					continue;
				}

				for (auto& k : horseManager->GetHorses())
				{
					//重複削除
					if (i == k || j == k)
						continue;

					betTicket.firstHorse = i->GetHorseNumber() - 1;
					betTicket.secondHorse = j->GetHorseNumber() - 1;
					betTicket.thirdHorse = k->GetHorseNumber() - 1;
					ticketManager->BetTicket(betTicket);
				}
			}
		}
	}
	ImGui::Text("price%d", ticketManager->GetMoney());
	ImGui::End();
}

void BetManager::BetTicketTypeSelectUpdate()
{
	if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_LEFT) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_LEFT))
	{
		currentSelectedBetCategory = BetManagerDefine::BetCategory(max((int)currentSelectedBetCategory - 1, 0));
		sound->Play("Select_Change");
	}
	else if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_RIGHT) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_RIGHT))
	{
		currentSelectedBetCategory = BetManagerDefine::BetCategory(min((int)currentSelectedBetCategory + 1, BetManagerDefine::CATEGORY_NUM - 1));
		sound->Play("Select_Change");
	}
	if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_Z) ||
		input->GetControllerInput()->GetPadInputPut(P_BUTTON_B))
	{
		sound->Play("Button_Select");
		ticketTypeSelect = true;
	}
}

void BetManager::BetHorseNumberSelectUpdate()
{
	//馬番選択のカーソルの位置の移動
	if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_LEFT) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_LEFT))
	{
		ticketHorseNumberCursorPos.x = max(ticketHorseNumberCursorPos.x - 1, 0);
		sound->Play("Select_Change");
	}
	else if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_RIGHT) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_RIGHT))
	{
		ticketHorseNumberCursorPos.x = min(ticketHorseNumberCursorPos.x + 1, TICKET_UI_LINE_HORSE_NUM - 1);
		sound->Play("Select_Change");
	}
	if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_UP) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_UP))
	{
		ticketHorseNumberCursorPos.y = max(ticketHorseNumberCursorPos.y - 1, 0);
		sound->Play("Select_Change");
	}
	else if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_DOWN) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_DOWN))
	{
		ticketHorseNumberCursorPos.y = min(ticketHorseNumberCursorPos.y + 1, horseManager->GetHorses().size() / TICKET_UI_LINE_HORSE_NUM);
		sound->Play("Select_Change");
	}

	if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_Z) ||
		input->GetControllerInput()->GetPadInputPut(P_BUTTON_B))
	{
		//馬番の最大数以下なら馬番の選択
		if (ticketHorseNumberCursorPos.y < horseManager->GetHorses().size() / TICKET_UI_LINE_HORSE_NUM)
			AddTicketSelectedHorse();
		else//それより奥なら馬券の決定ボタンを押す
		{
			//馬券を1枚以上購入しているなら次に進む
			if(selectedHorseNumbers.size()>0)
			{
				ticketContentDecide = true;
				SelectNumToTicketData(selectedHorseNumbers);
			}
			else
			{
				sound->Play("Cannot");
			}
		}

		sound->Play("Button_Select");
	}

	if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_X) ||
		input->GetControllerInput()->GetPadInputPut(P_BUTTON_A))
	{
		ticketTypeSelect = false;
		selectedHorseNumbers.clear();
	}
}

void BetManager::BetPriceSelectUpdate()
{
	//馬券選択のカーソルの位置の移動
	//横方向は購入のボタンがあるところがx=1でそれ以外は0
	if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_LEFT) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_LEFT))
	{
		ticketPriceCursorPos.x = max(ticketPriceCursorPos.x - 1, 0);
		sound->Play("Select_Change");
	}
	else if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_RIGHT) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_RIGHT))
	{
		ticketPriceCursorPos.x = min(ticketPriceCursorPos.x + 1, BET_TICKETS_LIST_DRAW_LINE);
		sound->Play("Select_Change");
	}

	if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_UP) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_UP) &&
		ticketPriceCursorPos.x == 0)
	{
		ticketPriceCursorPos.y = max(ticketPriceCursorPos.y - 1, 0);
		sound->Play("Select_Change");
	}
	else if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_DOWN) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_DOWN) &&
		ticketPriceCursorPos.x == 0)
	{
		ticketPriceCursorPos.y = min(ticketPriceCursorPos.y + 1, selectedBetTickets.size() - 1);
		sound->Play("Select_Change");
	}
	if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_Z) ||
		input->GetControllerInput()->GetPadInputPut(P_BUTTON_B))
	{
		ticketPriceSelect = true;
		sound->Play("Button_Select");//ToDo　馬券購入時のみ再生SEを変える
	}

	if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_X) ||
		input->GetControllerInput()->GetPadInputPut(P_BUTTON_A))
	{
		ticketContentDecide = false;
		//selectedHorseNumbers = TicketDataToSelectNum(selectedBetTickets);
		selectedBetTickets.clear();
	}
}

void BetManager::SelectTicketPriceChangeUpdate()
{
	//キャンセル処理
	if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_X) ||
		input->GetControllerInput()->GetPadInputPut(P_BUTTON_A))
	{
		ticketPriceSelect = false;
	}

	int selectedTicketIndex = GetTicketPriceCursorSelectedNum();
	if (selectedTicketIndex < 0)
		return;

	int maxPrice = (ticketManager->GetMoney() - GetSelectedTicketsPriceSum() + selectedBetTickets[selectedTicketIndex].price) / ticketManager->BET_MIN_MONEY * ticketManager->BET_MIN_MONEY;//指定できる最大の金額

	//馬券の購入金額を変更する
	if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_LEFT) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_LEFT))
	{
		selectedBetTickets[selectedTicketIndex].price = max(ticketManager->BET_MIN_MONEY, selectedBetTickets[selectedTicketIndex].price - ticketManager->BET_MIN_MONEY);
		sound->Play("Select_Change");
	}
	else if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_RIGHT) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_RIGHT))
	{
		selectedBetTickets[selectedTicketIndex].price = min(maxPrice, selectedBetTickets[selectedTicketIndex].price + ticketManager->BET_MIN_MONEY);
		sound->Play("Select_Change");
	}

	else if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_UP) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_UP) &&
		ticketPriceCursorPos.x == 0)
	{
		selectedBetTickets[selectedTicketIndex].price = min(maxPrice, selectedBetTickets[selectedTicketIndex].price + ticketManager->BET_MIN_MONEY * 10);
		sound->Play("Select_Change");
	}
	else if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_DOWN) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_DOWN) &&
		ticketPriceCursorPos.x == 0)
	{
		selectedBetTickets[selectedTicketIndex].price = max(ticketManager->BET_MIN_MONEY, selectedBetTickets[selectedTicketIndex].price - ticketManager->BET_MIN_MONEY * 10);
		sound->Play("Select_Change");
	}
}

void BetManager::AddTicketSelectedHorse()
{
	int addNum = GetCursorTicketHorseNumber();

	for (auto it = selectedHorseNumbers.begin(); it != selectedHorseNumbers.end(); it++)
	{
		//追加予定の番号が既にあったなら削除してループを終える
		if (addNum == *it)
		{
			selectedHorseNumbers.erase(it);
			return;
		}
	}

	selectedHorseNumbers.push_back(addNum);
}

const std::vector<TicketDefine::TicketData> BetManager::SelectNumToTicketData(const std::vector<int> _nums)
{
	std::vector<TicketDefine::TicketData> ret;

	for (const auto& n :_nums)
	{
		//2,3列目がある時の対応が出来ていないので後で修正する
		selectedBetTickets.push_back(TicketDefine::TicketData(ticketManager->BET_MIN_MONEY, GetSelectedTicketType(), n+1));
	}

	return ret;
}

const std::vector<int> BetManager::TicketDataToSelectNum(const std::vector<TicketDefine::TicketData> _tickets)
{
	std::vector<int> ret;
	for (const auto& t : _tickets)
	{
		//2,3列目がある時の対応が出来ていないので後で修正する
		ret.push_back(t.firstHorse-1);
	}

	return ret;
}

void BetManager::BuyTicket()
{
	//馬券の購入処理
	for(const auto& t:selectedBetTickets)
	{
		ticketManager->BetTicket(t);
	}

	//値のリセット
	BetDataReset();
}

int BetManager::GetSelectedTicketsPriceSum() const
{
	int sum = 0;
	for (const auto& t : selectedBetTickets)
		sum += t.price;

	return sum;
}

void BetManager::BetDataReset()
{
	ticketTypeSelect = false;
	ticketHorseNumberCursorPos = VECTOR2(0, 0);
	selectedHorseNumbers.clear();
	ticketContentDecide = false;
	ticketPriceCursorPos = VECTOR2(0, 0);
	ticketPriceSelect = false;
	currentSelectedBetCategory = BetManagerDefine::BETTING_TICKET;
	selectedBetTickets.clear();
}
