#pragma once
#include "Horse.h"

class InputManager;

class HorsePlayer :public Horse
{
public:
	HorsePlayer(VECTOR3 _pos, int _horseNumber, int _horseNum, int _model, COLOR_F  _color, const HorseParam& _param, CourseDataManager* _course, Animator* _anim, CommonManager* _common);
	~HorsePlayer();
	void Update()override;
	void GoalIn()override;

private:
	const float STICK_DEADZONE = 0.5f;
	InputManager* input;
};