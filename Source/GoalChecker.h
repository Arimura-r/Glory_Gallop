#pragma once
#include "../Library/GameObject.h"
#include <vector>

class Horse;
class HorseManager;
class CourseDataManager;
class GoalDataWriter;

class GoalChecker :public GameObject
{
public:
	GoalChecker(HorseManager* _runner, CourseDataManager* _course);
	~GoalChecker();
	void Update()override;
	void Draw()override;

private:
	HorseManager* horses;
	CourseDataManager* course;
	GoalDataWriter* goalData;

	//VECTOR3 goalPos1;
	//VECTOR3 goalPos2;

	//ゴールとの当たり判定を計算する
	//bool CollisionGoal(Horse* _horse);

	std::vector<int>orderFinish;//着順
	bool hasWrittenData;//データの書き込みを完了したかどうか
};