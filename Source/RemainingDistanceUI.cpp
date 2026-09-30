#include "RemainingDistanceUI.h"
#include "CourseDataManager.h"
#include "Horse.h"

namespace
{
	const unsigned int TEXT_COLOR = GetColor(255, 255, 255);
}

RemainingDistanceUI::RemainingDistanceUI(VECTOR2 _position, Horse* _target, CourseDataManager* _course):UIBase(_position,_target)
{
	course = _course;
	target = _target;
}

RemainingDistanceUI::~RemainingDistanceUI()
{
}

void RemainingDistanceUI::DrawUI()
{
	if (target == nullptr)
		return;

	float distance = target->GetRemainingDistance(); 
	int drawDistance = (int)distance / 100;

	DrawFormatStringF(position.x, position.y, TEXT_COLOR, "c‚è‹——£F%d m", drawDistance);
}
