#pragma once
#include "UIBase.h"

class WhipUI :public UIBase
{
public:
	WhipUI(VECTOR2 _position, Horse* _target) :UIBase(_position,_target){};
	~WhipUI();
	void DrawUI()override;
};