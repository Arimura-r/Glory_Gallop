#pragma once
#include "../Library/GameObject.h"
#include <vector>

class Horse;
class CourseDataManager;
class Stage;

class HorseCollisionManager :public GameObject
{
public:
	HorseCollisionManager(CourseDataManager* _course, Stage* _stage);
	~HorseCollisionManager();
	//馬同士の当たり判定
	void HorseToHorseCollisionUpdate(std::vector<Horse*>& _horses);
	//馬がコース外に出ないようにする判定
	void HorseToCourseCollisionUpdate(std::vector<Horse*>& _horses);
	//馬が地面から浮かずに接地出来るようにする
	void HorseToGroundCollisionUpdate(std::vector<Horse*>& _horses);
	//周りに他馬がいるかどうかの確認
	void CheckAroundHorse(std::vector<Horse*>& _horses);
	//進行方向に他の馬がいないかの確認
	//void VelocityCollisionUpdate(std::vector<Horse*>& _horses);

private:
	CourseDataManager* course;
	Stage* stage;
};