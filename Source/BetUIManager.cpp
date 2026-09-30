#include "BetUIManager.h"
#include "GameManager.h"
#include "BeforeRaceState.h"
#include "BetManager.h"
#include "BetTicketTypeSelectorUI.h"
#include "HaveMoneyUI.h"
#include "HaveBettingTicketsUI.h"
#include "BetTicketDecideUI.h"
#include "TicketWinOddsUI.h"
#include "TicketPlaceOddsUI.h"
#include "SelectedBetTicketList.h"
#include "TicketBuyUI.h"
#include "HorseManager.h"

namespace
{
	const VECTOR2 HAVE_MONEY_DRAW_OFFSET = VECTOR2(1200, 1000);
	const VECTOR2 HAVE_BETTING_TICKETS_OFFSET = VECTOR2(30, 250);
	const VECTOR2 TICKET_DECIDE_OFFSET = VECTOR2(200, 420);
	const VECTOR2 TICKET_ODDS_DRAW_OFFSET = VECTOR2(30, 300);
	const VECTOR2 SELECTED_TICKET_LIST_OFFSET=VECTOR2(30, 300);
	const VECTOR2 TICKET_BUY_DRAW_OFFSET = VECTOR2(500, 300);
}

BetUIManager::BetUIManager(VECTOR2 _position, GameManager* _gameManager, HorseManager* _horseManager, TicketManager* _ticket):UIBase(_position)
,gameManager(_gameManager),betManager(_gameManager->GetBeforeRaceState()->GetBetManager())
{
	int horseNum = (int)_horseManager->GetHorses().size();

	betUIList.push_back(new BetTicketTypeSelectorUI(_position, betManager));
	betUIList.push_back(new HaveMoneyUI(_position+HAVE_MONEY_DRAW_OFFSET, _ticket,gameManager));
	betUIList.push_back(new HaveBettingTicketsUI(_position + HAVE_BETTING_TICKETS_OFFSET, betManager, _ticket));
	betUIList.push_back(new BetTicketDecideUI(_position+TICKET_DECIDE_OFFSET, betManager));

	betUIList.push_back(new TicketWinOddsUI(_position + TICKET_ODDS_DRAW_OFFSET, betManager,_ticket, horseNum));
	betUIList.push_back(new TicketPlaceOddsUI(_position + TICKET_ODDS_DRAW_OFFSET, betManager, _ticket, horseNum));

	betUIList.push_back(new SelectedBetTicketListUI(_position + SELECTED_TICKET_LIST_OFFSET, betManager, _ticket));
	betUIList.push_back(new TicketBuyUI(_position + TICKET_BUY_DRAW_OFFSET, betManager));
}

BetUIManager::~BetUIManager()
{
}

void BetUIManager::DrawUI()
{
	if (gameManager->GetBeforeRaceState()->GetCurrentBeforeRaceSelect() != BeforeRaceState::TICKET)
		return;

	for (const auto& ui : betUIList)
	{
		ui->DrawUI();
	}
}
