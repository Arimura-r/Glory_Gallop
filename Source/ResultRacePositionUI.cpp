#include "ResultRacePositionUI.h"
#include "DrawNumFunc.h"
#include "Horse.h"
#include "GameManager.h"
#include "ResultState.h"

namespace
{
	const int NUM_WIDTH = 60;
	const int NUM_HEIGHT = 67;
	const float RACE_POSITION_TEXT_OFFSET_X = -500.0f;
	const float RACE_POSITION_TEXT_SCALE = 1.5f;
	const int LEFT_NUM_OFFSET_X = -150;
	const int RIGHT_NUM_OFFSET_X = 150;
	const float NUM_SCALE = 2.0f;
}

ResultRacePositionUI::ResultRacePositionUI(VECTOR2 _position, Horse* _target, GameManager* _gameManager, int _horseNum):UIBase(_position,_target)
{
	numImage = LoadGraph("data\\texture\\numberText.png");
	racePositionImage = LoadGraph("data\\texture\\racePositionText.png");
	horseNum = _horseNum;
	gameManager = _gameManager;
}
ResultRacePositionUI::~ResultRacePositionUI()
{
	DeleteGraph(numImage);
	numImage = -1;
	DeleteGraph(racePositionImage);
	racePositionImage = -1;
}
void ResultRacePositionUI::DrawUI()
{
	if (target == nullptr||gameManager->GetResultState()->GetCurrentDrawState()!=ResultState::PLAYER_GOAL_IN)
		return;

	DrawRotaGraphF(position.x + RACE_POSITION_TEXT_OFFSET_X, position.y,RACE_POSITION_TEXT_SCALE,0.0f, racePositionImage, true);//順位：　のテキスト
	DrawNumFunc::DrawRotaNum(position.x + LEFT_NUM_OFFSET_X, position.y, target->GetRacePosition()+1, NUM_WIDTH, numImage, NUM_WIDTH, NUM_HEIGHT, NUM_SCALE, 0.0f);//左側の自分の順位のテキスト　ToDo　GoalCheckerから順位は受け取るようにする
	DrawRectRotaGraphF(position.x, position.y, NUM_WIDTH * 10, 0, NUM_WIDTH, NUM_HEIGHT, NUM_SCALE, 0.0f, numImage, true);//順位の間の/のテキスト
	DrawNumFunc::DrawRotaNum(position.x+RIGHT_NUM_OFFSET_X, position.y, horseNum, NUM_WIDTH, numImage, NUM_WIDTH, NUM_HEIGHT, NUM_SCALE, 0.0f);//右側の全体の頭数のテキスト
}