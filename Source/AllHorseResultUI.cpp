#include "AllHorseResultUI.h"
#include "GameManager.h"
#include "ResultState.h"
#include "HorseManager.h"
#include "Horse.h"

namespace
{
	const int FONT_SIZE = 24;
	const int HORSE_INTERVAL = 40;//1頭ずつの着順表示の間隔
	const int BACK_BOX_WIDTH = 600;//背景の四角の横幅
	const int BACK_BOX_ALPHA = 128;
}

AllHorseResultUI::AllHorseResultUI(VECTOR2 _position, HorseManager* _horse, GameManager* _gameManager):UIBase(_position),
horseManager(_horse),gameManager(_gameManager)
{
}

AllHorseResultUI::~AllHorseResultUI()
{
}

void AllHorseResultUI::Update()
{
}

void AllHorseResultUI::DrawUI()
{
	int horseNum = (int)horseManager->GetHorses().size();

	//着順表示状態でないなら描画しない
	if (gameManager->GetResultState()->GetCurrentDrawState()!=ResultState::RACE_POSITION)
		return;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA,BACK_BOX_ALPHA);
	//テキストの背景に黒四角を置く
	DrawBox((int)position.x, (int)position.y, (int)position.x + BACK_BOX_WIDTH, (int)position.y + horseNum  * HORSE_INTERVAL, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,255);

	SetFontSize(FONT_SIZE);

	for (int i = 0; i < horseNum; i++)
	{
		const auto& h = horseManager->GetHorse(i);
		std::string racePosition = std::to_string(i+1) + "着";
		std::string horseNumber = std::to_string(h->GetHorseNumber()) + "番";
		std::string horseName = h->GetParam().name;
		//ToDo　人気、着差、タイムなども実装する
		
		DrawFormatStringF(position.x, position.y + HORSE_INTERVAL * i, 0xffffff, "%4s　%4s　%s", racePosition.c_str(), horseNumber.c_str(), horseName.c_str());
	}

	SetFontSize(DEFAULT_FONT_SIZE);
}
