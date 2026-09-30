#pragma once
#include "UIBase.h"

class Horse;
class HorseManager;

class RacePositionUI :public UIBase
{
public:
	RacePositionUI(VECTOR2 _position, Horse* _target,HorseManager* _horse);
	~RacePositionUI();

	void DrawUI()override;

private:
	HorseManager* horse;
};