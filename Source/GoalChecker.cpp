#include "GoalChecker.h"
#include "HorseManager.h"
#include "Horse.h"
#include "CourseDataManager.h"
#include "GoalDataWriter.h"
#include "BoxCollisionFunc.h"
#include "DrawFigureFunc.h"

GoalChecker::GoalChecker(HorseManager* _runner, CourseDataManager* _course)
{
	horses = _runner;
	course = _course;
	//goalPos1 = VECTOR3(2000, -100, 20000);
	//goalPos2 = VECTOR3(8000, 100, 19500);

	goalData = new GoalDataWriter();
	hasWrittenData = false;
}

GoalChecker::~GoalChecker()
{
}

void GoalChecker::Update()
{
	auto h = horses->GetHorses();

	auto hMax = h.size();
	for (int i = 0; i < hMax; i++)
	{
		//ToDo CourseDataの方のデータの形を変更　スタート、ゴールラインをVECTOR3～VECTOR3までの座標で記録する
		//if (not r[i]->GetIsGoal() && r[i]->GetPosition().z > course->GetData().goalLine)2D時のゴール判定
		if(not h[i]->GetIsGoal()&&h[i]->GetRemainingDistance()<0)
		{
			h[i]->GoalIn();
			orderFinish.push_back(i);
		}
	}

	//各馬の着順をcsvに書き込む
	if (orderFinish.size() == h.size()&&not hasWrittenData)
	{
		hasWrittenData = true;
		std::vector<int> allHorseOrderFinish;//各馬の着順を馬番順で並べたもの
		for (int i = 0; i < hMax; i++)
		{
			allHorseOrderFinish.push_back(h[i]->GetRacePosition());
		}
		goalData->DataAdd(allHorseOrderFinish);
	}
}

void GoalChecker::Draw()
{
	VECTOR3 goalInnerPos = course->GetGoalLineData().innerPos;
	VECTOR3 goalOuterPos = course->GetGoalLineData().innerPos + course->GetGoalLineData().lineVec * (course->GetOuterLine() - course->GetInnerLine());

	DrawLine3D(goalInnerPos + VECTOR3(0, 100, 0), goalOuterPos + VECTOR3(0, 100, 0), 0xff0000);
}

//bool GoalChecker::CollisionGoal(Horse* _horse)
//{
//	const VECTOR3& p = _horse->GetPosition();
//	if (BoxCollisionFunc::IsInBox(p, goalPos1, goalPos2))
//		return true;
//
//	return false;
//}
