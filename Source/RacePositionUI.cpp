#include "RacePositionUI.h"
#include "Horse.h"
#include "HorseManager.h"

namespace
{
	const unsigned int DEFAULT_TEXT_COLOR = GetColor(255, 255, 255);
	const unsigned int BOOSTING_TEXT_COLOR = GetColor(255, 128, 00);
}

RacePositionUI::RacePositionUI(VECTOR2 _position, Horse* _target, HorseManager* _horse):UIBase(_position,_target)
{
	horse = _horse;
}

RacePositionUI::~RacePositionUI()
{
}

void RacePositionUI::DrawUI()
{
	if (target == nullptr)
		return;

	int horseNum = (int)horse->GetHorses().size();
	int frontTargetPos = target->GetParam().boost.GetFrontChargeRange(horseNum) + 1;//(int)(target->GetParam().boost.frontChargeRange * (horseNum - 1)) + 1;
	int backTargetPos = target->GetParam().boost.GetBackChargeRange(horseNum) + 1;//(int)(target->GetParam().boost.backChargeRange * (horseNum - 1)) + 1;

	/*if (frontTargetPos != 1&&frontTargetPos!=18)
		frontTargetPos += 1;
	if (backTargetPos != 1&&backTargetPos != 18)
		backTargetPos += 1;*/

	unsigned int color = DEFAULT_TEXT_COLOR;

	if (target->GetParam().chargingBoost)
		color = BOOSTING_TEXT_COLOR;

	DrawFormatStringF(position.x, position.y, color, "%dˆÊ –Ú•W‡ˆÊF%dˆÊ`%dˆÊ", target->GetRacePosition()+1,frontTargetPos,backTargetPos);
}
