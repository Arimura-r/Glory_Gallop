#pragma once
#include "UIBase.h"

class Horse;

class StaminaUI :public UIBase
{
public:
	StaminaUI(VECTOR2 _position,Horse* _target);
	~StaminaUI();
	void DrawUI()override;

private:
};