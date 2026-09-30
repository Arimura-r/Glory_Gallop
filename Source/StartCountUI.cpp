#include "StartCountUI.h"
#include "GameManager.h"
#include "DrawNumFunc.h"

namespace
{
	int COUNT_DRAW_NUM = 3;//‰½•bˆÈ‰º‚È‚çƒJƒEƒ“ƒg‚Ì•`‰æ‚ðs‚¤‚©
	const int NUM_WIDTH = 60;
	const int NUM_HEIGHT = 67;
	const float NUM_EX_RATE = 2.0f;//”’l‰æ‘œ‚ÌŠg‘å—¦
}

StartCountUI::StartCountUI(VECTOR2 _position, GameManager* _gameManager):UIBase(_position,nullptr)
{
	gameManager = _gameManager;
	numImage = LoadGraph("data\\texture\\numberText.png");
}

StartCountUI::~StartCountUI()
{
	DeleteGraph(numImage);
	numImage = -1;
}

void StartCountUI::DrawUI()
{
	int num = (int)gameManager->GetRaceStartTime()+1;

	if (num > COUNT_DRAW_NUM)
		return;

	DrawNumFunc::DrawRotaNum(position.x, position.y, num, NUM_WIDTH, numImage, NUM_WIDTH, NUM_HEIGHT, NUM_EX_RATE, 0.0f);
}
