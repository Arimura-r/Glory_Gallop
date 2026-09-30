#include "ResultUIManager.h"
#include "GoalTextUI.h"
#include "TitleBackTextUI.h"
#include "ResultRacePositionUI.h"
#include "AllHorseResultUI.h"
#include "HorseManager.h"
#include "WaitHorseGoalUI.h"
#include "PressNextUI.h"
#include "PayOutResultUI.h"
#include "HitTicketUI.h"
#include "HaveMoneyUI.h"
#include "GameManager.h"
#include "ResultState.h"

namespace
{
	const float PLAYER_MODE_GOAL_TEXT_UI_Y = 300.0f;
	const float WATCH_MODE_GOAL_TEXT_UI_Y = 150.0f;
	const float RESULT_RACE_POSITION_UI_Y = 500.0f;
	const float PLAYER_MODE_TITLE_BACK_TEXT_UI_Y = 700.0f;
	const float WATCH_MODE_TITLE_BACK_TEXT_UI_Y = 1000.0f;

	const VECTOR2 ALL_HORSE_UI_POS = VECTOR2(450, 220);
	const VECTOR2 WAIT_HORSE_UI_POS = VECTOR2(450, 1000);
	const VECTOR2 PAY_OUT_UI_POS = VECTOR2(300, 100);
	const VECTOR2 HIT_TICKET_UI_POS = VECTOR2(300, 100);
	const VECTOR2 HAVE_MONEY_DRAW_OFFSET = VECTOR2(1400, 1000);
}

ResultUIManager::ResultUIManager(HorseManager* _horse, GameManager* _gameManager, TicketManager* _ticket, bool _canPlayerControll)
{
	VECTOR2 goalTextPos = VECTOR2(Screen::WIDTH_CENTER, PLAYER_MODE_GOAL_TEXT_UI_Y);
	VECTOR2 titleBackTextPos= VECTOR2(Screen::WIDTH_CENTER, PLAYER_MODE_TITLE_BACK_TEXT_UI_Y);
	
	if(_canPlayerControll)
	{
		uiList.push_back(new ResultRacePositionUI(VECTOR2(Screen::WIDTH_CENTER, RESULT_RACE_POSITION_UI_Y), (Horse*)_horse->GetPlayer(),_gameManager, (int)_horse->GetHorses().size()));
	}
	else
	{
		goalTextPos.y = WATCH_MODE_GOAL_TEXT_UI_Y;
		titleBackTextPos.y = WATCH_MODE_TITLE_BACK_TEXT_UI_Y;

		uiList.push_back(new AllHorseResultUI(ALL_HORSE_UI_POS, _horse,_gameManager));
		uiList.push_back(new WaitHorseGoalUI(WAIT_HORSE_UI_POS,_horse,_gameManager));
		uiList.push_back(new PressNextUI(titleBackTextPos, _gameManager));
		uiList.push_back(new PayOutResultUI(PAY_OUT_UI_POS, _gameManager, _horse,_ticket));
		uiList.push_back(new HitTicketUI(HIT_TICKET_UI_POS, _gameManager, _ticket));
		uiList.push_back(new HaveMoneyUI(HAVE_MONEY_DRAW_OFFSET, _ticket,_gameManager, false));
	}
	uiList.push_back(new GoalTextUI(goalTextPos,_gameManager));
	uiList.push_back(new TitleBackTextUI(titleBackTextPos, _gameManager));
}

ResultUIManager::~ResultUIManager()
{
}

void ResultUIManager::Update()
{
}

void ResultUIManager::DrawUI()
{
	for (const auto& u : uiList)
	{
		u->DrawUI();
	}
}
