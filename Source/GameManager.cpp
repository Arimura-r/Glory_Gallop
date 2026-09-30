#include "GameManager.h"
#include "../ImGui/imgui.h"
#include "HorseManager.h"
#include "Horse.h"
#include "CommonManager.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "BeforeRaceState.h"
#include "ResultState.h"
#include "TicketManager.h"

namespace
{
	float RACE_START_BEFORE_TIME = 5.0f;//レース開始までの時間
}

GameManager::GameManager(HorseManager* _horse, CommonManager* _common,TicketManager* _ticket)
{
	horse = _horse;
	player = (Horse*)_horse->GetPlayer();
	ticket = _ticket;
	raceTime = 0.0f;
	raceStartTime = RACE_START_BEFORE_TIME;
	state = BEFORE_RACE;
	input = _common->FindCommonObject<InputManager>();
	sound = _common->FindCommonObject<SoundManager>();
	changeAllViewToPlayCamera = false;
	
	beforeRace = new BeforeRaceState(input,sound,_ticket,_horse);
	result = new ResultState(input, sound,_horse,_ticket);
}

GameManager::~GameManager()
{
}

void GameManager::Update()
{
	switch (state)
	{
	case BEFORE_RACE:
		beforeRace->UpdateState();
		if (beforeRace->GetRequestStateChange())
		{
			sound->Play("Button_Select");
			state = BEFORE_PLAY;
			changeAllViewToPlayCamera = true;
		}
		break;
	case BEFORE_PLAY:
		BeforePlayUpdate();
		break;
	case IN_PLAY:
		InPlayUpdate();
		break;
	case RESULT:
		result->StateUpdate();
		break;
	default:
		break;
	}
}

void GameManager::BeforePlayUpdate()
{
	float beforeTime = raceStartTime;

	raceStartTime -= Time::DeltaTime();
	//前フレームからカウントの数値がズレているならカウントSEを鳴らす
	if ((int)beforeTime != (int)raceStartTime&&COUNT_PLAY_TIME>=raceStartTime)
	{
		sound->PlayOnce("Start_Count");
	}

	if (raceStartTime < 0.0f)
	{
		state = IN_PLAY;
		horse->RaceStart();
		raceStartTime = 0.0f;

		sound->PlayOnce("Gate_Open");
		sound->Play("Footstep", 50, true);
		sound->Play("Play_BGM", 50, true);
		sound->Play("Wind", 50, true);
	}
}

void GameManager::InPlayUpdate()
{
	//時間経過
	raceTime += Time::DeltaTime();

	//リザルトへ遷移する処理
	if (state == IN_PLAY)
	{
		//プレイヤーがいるならプレイヤーがゴールすることを遷移条件、
		//プレイヤーがいないなら先頭の馬がゴールすることを遷移条件にする
		if((player != nullptr && player->GetIsGoal()) || 
			player == nullptr && horse->GetHorse(0)->GetIsGoal())
		{
			state = RESULT;
			sound->Stop("Footstep");
		}
		//sound->ChangeVolume(25, "Play_BGM");
	}
}
