#include "GoalTextUI.h"
#include "GameManager.h"
#include "ResultState.h"

namespace
{
	const float TEXT_DEFAULT_SCALE = 1.0f;
}

GoalTextUI::GoalTextUI(VECTOR2 _position, GameManager* _gameManager):UIBase(_position,nullptr)
{
	hImage = LoadGraph("data\\texture\\goalText.png");
	scale = TEXT_DEFAULT_SCALE;
	gameManager = _gameManager;
}

GoalTextUI::~GoalTextUI()
{
	DeleteGraph(hImage);
}

void GoalTextUI::DrawUI()
{
	//プレイヤーゴール後か着順表示状態でのみ表示を行う
	if (gameManager->GetResultState()->GetCurrentDrawState() != ResultState::RACE_POSITION &&
		gameManager->GetResultState()->GetCurrentDrawState() != ResultState::PLAYER_GOAL_IN)
		return;

	DrawRotaGraphF(position.x, position.y, scale, 0.0f, hImage, true);
}
