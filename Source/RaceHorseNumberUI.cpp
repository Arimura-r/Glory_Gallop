#include "RaceHorseNumberUI.h"
#include "HorseManager.h"
#include "Horse.h"
#include "GameManager.h"
#include "Camera.h"

namespace
{
	const VECTOR3 HORSE_POS_OFFSET = VECTOR3(0, 250,0);//馬の座標から見てどの位置に表示を行うか

	const VECTOR2 BACK_BOX_SIZE = VECTOR2(28, 28);
	const int BACK_BOX_ALPHA = 128;

	const int FONT_SIZE = 24;

	const float DRAW_CAMERA_DISTANCE = 6000.0f;//カメラとの距離がこれ以上なら表示を行わない
}

RaceHorseNumberUI::RaceHorseNumberUI(HorseManager* _horseManager, GameManager* _gameManager, Camera* _camera)
	:horseManager(_horseManager),gameManager(_gameManager),camera(_camera)
{
}

RaceHorseNumberUI::~RaceHorseNumberUI()
{
}

void RaceHorseNumberUI::DrawUI()
{
	//プレイヤーがいない観戦モード時のみ表示を行う
	if (horseManager->GetPlayer() != nullptr)
		return;

	SetFontSize(FONT_SIZE);

	for (const auto& h : horseManager->GetHorses())
	{
		VECTOR3 pos = ConvWorldPosToScreenPos(h->GetPosition()+HORSE_POS_OFFSET);

		//カメラとの距離が一定以上なら表示しない
		if (VSquareSize(camera->GetCameraPosition() - h->GetPosition() + HORSE_POS_OFFSET) > DRAW_CAMERA_DISTANCE * DRAW_CAMERA_DISTANCE)
			continue;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, BACK_BOX_ALPHA);
		//テキストの背景に黒四角を置く
		DrawBox((int)pos.x, (int)pos.y, (int)pos.x + (int)BACK_BOX_SIZE.x, (int)pos.y + (int)BACK_BOX_SIZE.y, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

		DrawFormatStringF(pos.x, pos.y, 0xffffff, "%d", h->GetHorseNumber());
	}

	SetFontSize(DEFAULT_FONT_SIZE);
}
