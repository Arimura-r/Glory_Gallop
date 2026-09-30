#include "BetTicketTypeSelectorUI.h"
#include "TicketManager.h"
#include "BetManager.h"

namespace
{
	const VECTOR2 IMAGE_SIZE = VECTOR2(180, 120);
}

BetTicketTypeSelectorUI::BetTicketTypeSelectorUI(VECTOR2 _position, BetManager* _betManager):UIBase(_position),betManager(_betManager)
{
	std::string texPath = "data/texture/";
	ticketImageHandle.emplace(BetManagerDefine::BETTING_TICKET, LoadGraph((texPath + "bettingTicketText.png").c_str()));
	ticketImageHandle.emplace(BetManagerDefine::WIN, LoadGraph((texPath + "ticketWinText.png").c_str()));
	ticketImageHandle.emplace(BetManagerDefine::PLACE, LoadGraph((texPath + "ticketPlaceText.png").c_str()));
	ticketImageHandle.emplace(BetManagerDefine::QUINELLA, LoadGraph((texPath + "ticketQuinellaText.png").c_str()));
	ticketImageHandle.emplace(BetManagerDefine::EXACTA, LoadGraph((texPath + "ticketExactaText.png").c_str()));
	ticketImageHandle.emplace(BetManagerDefine::QUINELLA_PLACE, LoadGraph((texPath + "ticketQuinellaPlaceText.png").c_str()));
	ticketImageHandle.emplace(BetManagerDefine::TRIO, LoadGraph((texPath + "ticketTrioText.png").c_str()));
	ticketImageHandle.emplace(BetManagerDefine::TRIFECTA, LoadGraph((texPath + "ticketTrifectaText.png").c_str()));
}

BetTicketTypeSelectorUI::~BetTicketTypeSelectorUI()
{
	for (auto& h : ticketImageHandle)
	{
		DeleteGraph(h.second);
	}
}

void BetTicketTypeSelectorUI::DrawUI()
{
	for (int i = 0; i < BetManagerDefine::CATEGORY_NUM; i++)
	{
		auto it = ticketImageHandle.find(i);
		//要素が見つからなかったらスキップ
		if (it == ticketImageHandle.end())
			continue;

		float addY = 0.0f;
		//選択されている項目だけ右に出す
		if (betManager->GetCurrentSelectedBetCategory() == i)
			addY += 100.0f;

		DrawGraphF(position.x + IMAGE_SIZE.x * i, position.y + addY, (*it).second, true);
	}
}
