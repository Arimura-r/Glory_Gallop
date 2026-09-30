#include "PlayUIManager.h"
#include "RaceTimeUI.h"
#include "RemainingDistanceUI.h"
#include "RacePositionUI.h"
#include "SpeedUI.h"
#include "StaminaUI.h"
#include "WhipUI.h"
#include "HorseDataUI.h"

#include "Camera.h"
#include "HorseManager.h"
#include "Horse.h"

namespace
{
	const VECTOR2 RACE_TIME_UI_POS = VECTOR2(1700, 100);
	const VECTOR2 HORSE_DATA_UI_POS = VECTOR2(200, 50);
	const VECTOR2 REMAINING_DISTANCE_UI_POS = VECTOR2(200, 100);
	const VECTOR2 RACE_POSITION_UI_POS = VECTOR2(200, 150);
	const VECTOR2 SPEED_UI_POS = VECTOR2(200, 200);
	const VECTOR2 STAMINA_UI_POS = VECTOR2(200, 250);
	const VECTOR2 WHIP_UI_POS = VECTOR2(400, 100);
}

PlayUIManager::PlayUIManager(GameManager* _gameManager, CourseDataManager* _course, HorseManager* _horse, Camera* _camera):camera(_camera)
{
	Horse* player = (Horse*)_horse->GetPlayer();
	uiList.push_back(new RaceTimeUI(RACE_TIME_UI_POS,_gameManager));
	uiList.push_back(new HorseDataUI(HORSE_DATA_UI_POS, player));
	uiList.push_back(new RemainingDistanceUI(REMAINING_DISTANCE_UI_POS,player, _course));
	uiList.push_back(new RacePositionUI(RACE_POSITION_UI_POS,player,_horse));
	uiList.push_back(new SpeedUI(SPEED_UI_POS,player));
	uiList.push_back(new StaminaUI(STAMINA_UI_POS, player));
	uiList.push_back(new WhipUI(WHIP_UI_POS, player));
}

PlayUIManager::~PlayUIManager()
{
}

void PlayUIManager::DrawUI()
{
	ChangeUITarget();

	for (const auto& u : uiList)
	{
		u->DrawUI();
	}
}

void PlayUIManager::ChangeUITarget()
{
	for (auto& ui : uiList)
	{
		ui->SetTarget(camera->GetCameraTargetHorse());
	}
}
