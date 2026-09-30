#include "HorseDataUI.h"
#include "Horse.h"

namespace
{
	const unsigned int TEXT_COLOR = GetColor(255, 255, 255);
}

HorseDataUI::HorseDataUI(VECTOR2 _position, Horse* _target) :UIBase(_position, _target)
{

}

HorseDataUI::~HorseDataUI()
{
}

void HorseDataUI::DrawUI()
{
	if (target == nullptr)
		return;

	DrawFormatStringF(position.x, position.y,TEXT_COLOR , "%d”Ô@%s", target->GetHorseNumber(), target->GetParam().name.c_str());
}
