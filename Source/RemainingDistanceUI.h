#pragma once
#include "UIBase.h"

class CourseDataManager;
class Horse;

//Žc‚è‹——£‚ð•\Ž¦‚·‚é
class RemainingDistanceUI :public UIBase
{
public:
	RemainingDistanceUI(VECTOR2 _position,Horse* _target,CourseDataManager* _course);
	~RemainingDistanceUI();

	void DrawUI()override;

private:
	CourseDataManager* course;
};