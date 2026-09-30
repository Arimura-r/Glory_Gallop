#include "ResultState.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "HorseManager.h"
#include "Horse.h"
#include "TicketManager.h"

ResultState::ResultState(InputManager* _input, SoundManager* _sound, HorseManager* _horse, TicketManager* _ticket):
	input(_input),sound(_sound),horseManager(_horse),ticketManager(_ticket),titleBackFlg(false)
{
	//プレイヤーの有無で初期ステートを変更
	if (horseManager->GetPlayer())
		state = PLAYER_GOAL_IN;
	else
		state = GOAL_WAIT;
}

ResultState::~ResultState()
{
}

void ResultState::StateUpdate()
{
	HorsePlayer* player = horseManager->GetPlayer();//プレイヤーがいるなら操作モード、いないなら観戦モード

	switch (state)
	{
	case ResultState::GOAL_WAIT:
		GoalWaitStateUpdate();
		break;
	case ResultState::RACE_POSITION:
		RacePositionStateUpdate();
		break;
	case ResultState::PAY_OUT_RESULT:
		PayOutResultStateUpdate();
		break;
	case ResultState::TICKET_HIT:
		TicketHitStateUpdate();
		TitleBackUpdate();
		break;
	case ResultState::PLAYER_GOAL_IN:
		PlayerGoalInUpdate();
		TitleBackUpdate();
		break;
	default:
		assert(false && "異常な描画ステートに入っています");
		break;
	}
}

void ResultState::GoalWaitStateUpdate()
{
	if ((CheckHitKey(KEY_INPUT_Z) || input->GetControllerInput()->GetPadInputPushing(P_BUTTON_B)))
		Time::ChangeSpeedRate(5.0f);
	else
		Time::ChangeSpeedRate(1.0f);

	//全馬がゴールしたら着順表示を行う
	if (horseManager->GetHorse((int)horseManager->GetHorses().size() - 1)->GetIsGoal())
	{
		state = RACE_POSITION;
		Time::ChangeSpeedRate(1.0f);
	}
}

void ResultState::RacePositionStateUpdate()
{
	//Bを押したら払い戻し表示へ移行
	if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_Z) ||
		input->GetControllerInput()->GetPadInputPut(P_BUTTON_B))
	{
		state = PAY_OUT_RESULT;
		sound->Play("Button_Select");
	}
}

void ResultState::PayOutResultStateUpdate()
{
	//Bを押したら的中馬券表示へ移行
	if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_Z) ||
		input->GetControllerInput()->GetPadInputPut(P_BUTTON_B))
	{
		state = TICKET_HIT;
		sound->Play("Button_Select");

		//払い戻しを行う
		ticketManager->PayOut();
	}
}

void ResultState::TicketHitStateUpdate()
{
	//特になし　Aを押したらタイトルに戻る
}

void ResultState::PlayerGoalInUpdate()
{
}

void ResultState::TitleBackUpdate()
{
	if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_X) ||
		input->GetControllerInput()->GetPadInputPut(P_BUTTON_A))
	{
		sound->Play("Button_Select");
		titleBackFlg = true;

	}
	if (titleBackFlg && not sound->CheckPlay("Button_Select"))
	{
		SceneManager::ChangeScene("TITLE");
	}
}
