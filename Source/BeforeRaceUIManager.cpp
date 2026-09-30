#include "BeforeRaceUIManager.h"
#include "UIBase.h"
#include "DrawDataSelectorUI.h"
#include "AllHorseDataUI.h"
#include "HorseInfoUI.h"
#include "RaceDataUI.h"
#include "BetUIManager.h"
#include "RaceStartUI.h"
#include "TicketManager.h"

namespace
{
	const VECTOR2 DRAW_DATA_SELECTOR_UI_POS = VECTOR2(0, 100);
	const VECTOR2 ALL_HORSE_DATA_UI_POS = VECTOR2(500, 150);
	const VECTOR2 HORSE_INFO_UI_POS = VECTOR2(950, 150);
	const VECTOR2 RACE_DATA_UI_POS = VECTOR2(500, 300);
	const VECTOR2 BET_UI_POS = VECTOR2(300, 0);
	const float RACE_START_UI_Y = 900.0f;
}

BeforeRaceUIManager::BeforeRaceUIManager(HorseManager* _horse, GameManager* _gameManager, CourseDataManager* _course, TicketManager* _ticket)
{
	uiList.push_back(new DrawDataSelectorUI(DRAW_DATA_SELECTOR_UI_POS, _gameManager, _horse));
	uiList.push_back(new AllHorseDataUI(ALL_HORSE_DATA_UI_POS, _horse, _gameManager));
	uiList.push_back(new HorseInfoUI(HORSE_INFO_UI_POS, _horse, _gameManager,_ticket));
	uiList.push_back(new RaceDataUI(RACE_DATA_UI_POS, _course, _horse, _gameManager));
	uiList.push_back(new BetUIManager(BET_UI_POS,_gameManager,_horse,_ticket));
	uiList.push_back(new RaceStartUI(VECTOR2(Screen::WIDTH_CENTER, RACE_START_UI_Y), _gameManager));
}

BeforeRaceUIManager::~BeforeRaceUIManager()
{
}

void BeforeRaceUIManager::Update()
{
}

void BeforeRaceUIManager::DrawUI()
{
	for (const auto& ui : uiList)
	{
		ui->DrawUI();
	}
}
