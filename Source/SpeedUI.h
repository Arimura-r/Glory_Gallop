#pragma once
#include "UIBase.h"

class Horse;

class SpeedUI :public UIBase
{
public:
	SpeedUI(VECTOR2 _position,Horse* _target);
	~SpeedUI();
	void DrawUI()override;

private:
};