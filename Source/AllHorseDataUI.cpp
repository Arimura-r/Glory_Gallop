#include "AllHorseDataUI.h"
#include "HorseManager.h"
#include "Horse.h"
#include "GameManager.h"
#include "BeforeRaceState.h"
#include "HorseDataDrawManager.h"

namespace
{
	const int FONT_SIZE = 24;
	const int HORSE_INTERVAL = 40;//1頭ずつの着順表示の間隔
	const int BACK_BOX_WIDTH = 400;//背景の四角の横幅
	const int BACK_BOX_ALPHA = 128;
}

AllHorseDataUI::AllHorseDataUI(VECTOR2 _position, HorseManager* _horse, GameManager* _game):UIBase(_position)
	,horseManager(_horse),gameManager(_game)
{
}

AllHorseDataUI::~AllHorseDataUI()
{
}

void AllHorseDataUI::Update()
{
}

void AllHorseDataUI::DrawUI()
{
	//出走馬データが選ばれていない場合描画しない
	if (gameManager->GetBeforeRaceState()->GetCurrentBeforeRaceSelect() != BeforeRaceState::ALL_HORSE_DATA)
		return;

	int horseNum = (int)horseManager->GetHorses().size();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BACK_BOX_ALPHA);
	//テキストの背景に黒四角を置く
	DrawBox((int)position.x, (int)position.y, (int)position.x + BACK_BOX_WIDTH, (int)position.y + horseNum * HORSE_INTERVAL, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	SetFontSize(FONT_SIZE);

	const auto& horses = horseManager->GetHorses();
	for (int i = 0; i < horseNum; i++)
	{
		const auto& h = horses[i];
		std::string horseNumber = std::to_string(h->GetHorseNumber()) + "番";
		std::string horseName = h->GetParam().name;
		//ToDo　人気、着差、タイムなども実装する

		DrawFormatStringF(position.x, position.y + HORSE_INTERVAL * i, 0xffffff, "%4s　%s",  horseNumber.c_str(), horseName.c_str());
	}

	SetFontSize(DEFAULT_FONT_SIZE);

	//カーソルの表示
	int cursorY = (int)position.y + HORSE_INTERVAL * gameManager->GetBeforeRaceState()->GetHorseDataDrawManager()->GetCursorHorseNum();
	DrawBox((int)position.x, cursorY, (int)position.x + BACK_BOX_WIDTH, cursorY + HORSE_INTERVAL, 0xffffff, false);
}
