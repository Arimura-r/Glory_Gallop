#include "RaceTimeUI.h"
#include "GameManager.h"
#include "TimeStringFunc.h"

namespace
{
	const unsigned int TEXT_COLOR = GetColor(255, 255, 255);
}

RaceTimeUI::RaceTimeUI(VECTOR2 _position, GameManager* _gameManager):UIBase(_position)
{
	gameManager = _gameManager;
}

RaceTimeUI::~RaceTimeUI()
{
}

void RaceTimeUI::DrawUI()
{
	std::string str = TimeStringFunc::GetTimeString(gameManager->GetRaceTime(), '.').c_str();
	DrawFormatStringF(position.x, position.y, TEXT_COLOR, "%s", str.c_str());
}
