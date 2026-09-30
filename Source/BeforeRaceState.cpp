#include "BeforeRaceState.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "BetManager.h"
#include "HorseManager.h"
#include "HorseDataDrawManager.h"

BeforeRaceState::BeforeRaceState(InputManager* _input, SoundManager* _sound, TicketManager* _ticket, HorseManager* _horse):
	input(_input),sound(_sound),horse(_horse), currentSelect(RACE_DATA), requestStateChange(false)
{
	horseData = new HorseDataDrawManager(_horse, _input, _sound);

	bet = nullptr;
	//ticketクラスが存在するならインスタンスを生成する(プレイヤー不在時のみ生成する)
	if(_ticket!=nullptr)
		bet = new BetManager(_ticket,_horse,input,sound);
}

BeforeRaceState::~BeforeRaceState()
{
}

void BeforeRaceState::UpdateState()
{
	SelectorControll();

	switch (currentSelect)
	{
	case BeforeRaceState::RACE_DATA:
		break;
	case BeforeRaceState::ALL_HORSE_DATA:
		horseData->HorseDataUpdate();
		break;
	case BeforeRaceState::TICKET:
		bet->BetUpdate();
		break;
	case BeforeRaceState::START:
		//ゲームの開始処理
		if (CheckHitKey(KEY_INPUT_Z) || input->GetControllerInput()->GetPadInputPushing(P_BUTTON_B))
			requestStateChange = true;
		break;
	default:
		break;
	}	
}

void BeforeRaceState::SelectorControll()
{
	//馬券の種類が決定されている時は移動を停止する
	if (bet != nullptr && bet->GetIsTicketTypeSelected())
		return;

	//選択している項目の変更
	if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_UP) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_UP))
	{
		currentSelect = BeforeRaceSelected(max((int)currentSelect - 1, 0));
		sound->Play("Select_Change");

		//プレイヤー操作モードなら馬券の項目を選べないようにする
		if (horse->GetPlayer() != nullptr && currentSelect == TICKET)
			currentSelect = BeforeRaceSelected(TICKET - 1);
	}
	else if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_DOWN) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_DOWN))
	{
		currentSelect = BeforeRaceSelected(min((int)currentSelect + 1, SELECT_NUM - 1));
		sound->Play("Select_Change");

		//プレイヤー操作モードなら馬券の項目を選べないようにする
		if (horse->GetPlayer() != nullptr && currentSelect == TICKET)
			currentSelect = BeforeRaceSelected(TICKET + 1);
	}
}
