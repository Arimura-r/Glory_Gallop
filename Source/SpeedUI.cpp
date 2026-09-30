#include "SpeedUI.h"
#include "Horse.h"

namespace
{
	const unsigned int TEXT_COLOR = GetColor(255, 255, 255);
}

SpeedUI::SpeedUI(VECTOR2 _position, Horse* _target):UIBase(_position,_target)
{
	target = _target;
}

SpeedUI::~SpeedUI()
{
}

void SpeedUI::DrawUI()
{
	if (target == nullptr)
		return;

	float speed = target->GetSpeedParam().currentSpeed / 100.0f;//秒速(メートル)
	speed *= 3600.0f / 1000.0f;//上の値を時速(キロメートル)に変換

	DrawFormatStringF(position.x, position.y, TEXT_COLOR, "%.1f km/h",speed);
}
