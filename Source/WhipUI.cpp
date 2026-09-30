#include "WhipUI.h"
#include "Horse.h"

namespace
{
	const unsigned int DEFAULT_TEXT_COLOR = GetColor(255, 255, 255);
	const unsigned int WHIPPING_TEXT_COLOR = GetColor(0, 255, 0);
}

WhipUI::~WhipUI()
{
}

void WhipUI::DrawUI()
{
	if (target == nullptr)
		return;

	std::string whipStr = "ï⁄ÅF";
	for (int i = 0; i < target->GetParam().whipNum; i++)
		whipStr += "Åú";
	unsigned int color = DEFAULT_TEXT_COLOR;
	if (target->GetWhipping())
		color = WHIPPING_TEXT_COLOR;

	DrawFormatStringF(position.x, position.y, color, whipStr.c_str());
}
