#include "HorseDataDrawManager.h"
#include "InputManager.h"
#include "HorseManager.h"
#include "SoundManager.h"

HorseDataDrawManager::HorseDataDrawManager(HorseManager* _horse, InputManager* _input, SoundManager* _sound):
	cursorHorseNum(0),horseManager(_horse),input(_input),sound(_sound)
{
}

HorseDataDrawManager::~HorseDataDrawManager()
{
}

void HorseDataDrawManager::HorseDataUpdate()
{
	//馬番選択のカーソルの位置の移動
	if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_LEFT) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_LEFT))
	{
		cursorHorseNum = max(0, cursorHorseNum - 1);
		sound->Play("Select_Change");
	}
	else if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_RIGHT) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_RIGHT))
	{
		cursorHorseNum = min(cursorHorseNum+1, (int)horseManager->GetHorses().size()-1);
		sound->Play("Select_Change");
	}
}

