#include "WaitHorseGoalUI.h"
#include "HorseManager.h"
#include "Horse.h"
#include "GameManager.h"
#include "ResultState.h"

namespace
{
	const int FONT_SIZE = 32;
}

WaitHorseGoalUI::WaitHorseGoalUI(VECTOR2 _position, HorseManager* _horseManager, GameManager* _gameManager):UIBase(_position)
,horseManager(_horseManager),gameManager(_gameManager)
{
}

WaitHorseGoalUI::~WaitHorseGoalUI()
{
}

void WaitHorseGoalUI::Update()
{
}

void WaitHorseGoalUI::DrawUI()
{
	int horseNum = (int)horseManager->GetHorses().size();
	int goalHorseNum = horseManager->GetGoalHorseNum();

	//全馬がゴールしているなら
	if (horseNum == goalHorseNum || gameManager->GetResultState()->GetCurrentDrawState() != ResultState::GOAL_WAIT)
		return;

	SetFontSize(FONT_SIZE);
	DrawFormatStringF(position.x, position.y, 0xffffff, "全馬がゴールするまでお待ちください(%2d / %2d)\n(Bボタン長押しで倍速出来ます)", goalHorseNum, horseNum);
	SetFontSize(DEFAULT_FONT_SIZE);
}
