#pragma once
#include "UIBase.h"

class HorseDataUI :public UIBase
{
public:
	HorseDataUI(VECTOR2 _position,Horse* _target);
	~HorseDataUI();

	void DrawUI()override;
};