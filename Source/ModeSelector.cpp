#include "ModeSelector.h"
#include "../ImGui/imgui.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "Screen.h"

namespace
{
	const float SELECT_MODE_TEXT_HEIGHT = 300.0f;
	const float SELECT_MODE_TEXT_EXRATE = 1.5f;  
	const float ARROW_CENTER_DISTANCE = 400.0f;//左右の矢印の中央からの距離
	const float ARROW_EXRATE = 2.5f;

	const float MODE_TEXT_HEIGHT = 800.0f;
	const float MODE_TEXT_EXRATE = 1.0f;
}

ModeSelector::ModeSelector(int _playTextHandle, InputManager* _input, SoundManager* _sound):
	canPlayerControll(false),active(false),playReady(false),playTextHandle(_playTextHandle),input(_input),sound(_sound)
{
	std::string texFilePath = "data/texture/";
	leftArrowHandle = LoadGraph((texFilePath + "leftArrowText.png").c_str());
	rightArrowHandle = LoadGraph((texFilePath + "rightArrowText.png").c_str());
	selectModeTextHandle = LoadGraph((texFilePath + "selectmodeText.png").c_str());
	rideModeTextHandle = LoadGraph((texFilePath + "ridemodeText.png").c_str());
	watchModeTextHandle = LoadGraph((texFilePath + "watchmodeText.png").c_str());
}

ModeSelector::~ModeSelector()
{
	DeleteGraph(leftArrowHandle);
	DeleteGraph(rightArrowHandle);
	DeleteGraph(selectModeTextHandle);
	DeleteGraph(rideModeTextHandle);
	DeleteGraph(watchModeTextHandle);
}

void ModeSelector::Update()
{
	if (not active || playReady)
		return;

	//もう1回決定ボタンを押したらゲームスタート
	if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_Z) ||
		input->GetControllerInput()->GetIsButtonPutNow(P_BUTTON_B))
	{
		playReady = true;
		sound->Play("Button_Select");
	}

	//左右どちらかが押されたらモードを変更する
	if (input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_RIGHT) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_RIGHT) ||
		input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_LEFT) ||
		input->GetControllerInput()->GetPadInputPut(P_L_STICK_LEFT))
	{
		canPlayerControll = not canPlayerControll;
		sound->Play("Select_Change");
	}
}

void ModeSelector::SelectorDraw()
{
	if (not active)
		return;

	DrawRotaGraphF(Screen::WIDTH_CENTER, SELECT_MODE_TEXT_HEIGHT, SELECT_MODE_TEXT_EXRATE, 0.0f, selectModeTextHandle, true);
	DrawRotaGraphF(Screen::WIDTH_CENTER - ARROW_CENTER_DISTANCE, MODE_TEXT_HEIGHT, ARROW_EXRATE, 0.0f, leftArrowHandle, true);
	DrawRotaGraphF(Screen::WIDTH_CENTER + ARROW_CENTER_DISTANCE, MODE_TEXT_HEIGHT, ARROW_EXRATE, 0.0f, rightArrowHandle, true);

	int modeTextHandle = watchModeTextHandle;
	if (canPlayerControll)
		modeTextHandle = rideModeTextHandle;
	DrawRotaGraphF(Screen::WIDTH_CENTER, MODE_TEXT_HEIGHT, MODE_TEXT_EXRATE, 0.0f, modeTextHandle, true);

	//DrawGraph(600, 0, rideModeTextHandle, true);
	//DrawGraph(800, 0, watchModeTextHandle, true);
}
