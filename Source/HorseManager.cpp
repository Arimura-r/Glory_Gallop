#include "HorseManager.h"
#include "HorseComputer.h"
#include "HorsePlayer.h"
#include "HorseParamReader.h"
#include "CourseDataManager.h"
#include "HorseAnimLoader.h"
#include "HorseModelLoader.h"
#include "HorseCollisionManager.h"
#include "HorseCommentData.h"
#include "CommonManager.h"
#include "../ImGui/imgui.h"
#include <map>

namespace
{
	//プレイヤーの枠番　ToDo 外部から指定できるようにする
	const int PLAYER_GATE_NUMBER = 9;
	//float horseGateInterval = 160.0f;//スタート時の馬同士の距離
}

HorseManager::HorseManager(CourseDataManager* _course, CommonManager* _common, Stage* _stage, bool _canPlayerControll)
{
	HorseParamReader* param=new HorseParamReader();
	model = new HorseModelLoader();
	anim = new HorseAnimLoader();
	collision = new HorseCollisionManager(_course,_stage);

	course = _course;
	player = nullptr;
	goalHorseNum = 0;
	CourseDataManager::LineData startData = _course->GetStartLineData();

	for (int i = 0; i < param->GetParams().size(); i++)
	{
		Horse* h = nullptr;
		if (i == PLAYER_GATE_NUMBER-1&&_canPlayerControll)
		{
			//player = new HorsePlayer(VECTOR3(i * horseGateInterval + _course->GetLeftCornerCenter().x + _course->GetInnerLine(),0,_course->GetStartLine()), i+1, model->GetDupliModel(),colors[i], param->GetParams()[i], course, anim->GetAnimDupli(), _common);
			player = new HorsePlayer(startData.innerPos + (startData.lineVec  *_course->GetStartHorseDistance() * (float)i), i + 1,(int)param->GetParams().size(), model->GetDupliModel(HorseModelLoader::BodyColor(param->GetParams()[i].bodyTextureIndex)), colors[i], param->GetParams()[i], course, anim->GetAnimDupli(), _common);
			h = player;
		}
		else
		{
			//h = new HorseComputer(VECTOR3(i * horseGateInterval + _course->GetLeftCornerCenter().x + _course->GetInnerLine(),0 , _course->GetStartLine()), i+1, model->GetDupliModel(), colors[i],param->GetParams()[i], course, anim->GetAnimDupli(), _common);
			h = new HorseComputer(startData.innerPos + (startData.lineVec  * _course->GetStartHorseDistance() * (float)i), i + 1, (int)param->GetParams().size(), model->GetDupliModel(HorseModelLoader::BodyColor(param->GetParams()[i].bodyTextureIndex)), colors[i], param->GetParams()[i], course, anim->GetAnimDupli(), _common);
		}
		horses.push_back(h);
	}
	comment = new HorseCommentData(horses);

	delete anim;
}

HorseManager::~HorseManager()
{
}

void HorseManager::Update()
{
	//各種当たり判定の更新
	collision->HorseToHorseCollisionUpdate(horses);
	collision->HorseToCourseCollisionUpdate(horses);
	collision->HorseToGroundCollisionUpdate(horses);
	//collision->VelocityCollisionUpdate(horses);
	//馬の現在地点の更新
	for (auto& h : horses)
	{
		h->PositionUpdate();
	}
	collision->CheckAroundHorse(horses);
	//順位の更新
	auto racePositionOrder=RacePositionUpdate();

	//各馬の前にいる馬の情報を与える
	for (int i=0;i<racePositionOrder.size();i++)
	{
		Horse* prevHorse = nullptr;
		Horse* behindHorse = nullptr;
		if (i > 0)
			prevHorse=racePositionOrder[i - 1].horse;
		if (i < racePositionOrder.size() - 1)
			behindHorse = racePositionOrder[i + 1].horse;

		racePositionOrder[i].horse->PrevPositionHorseCheck(prevHorse);
		racePositionOrder[i].horse->BehindPositionHorseCheck(behindHorse);
	}

#if _DEBUG
	ImGui::Begin("Horses");
	auto max = horses.size();
	for (int i = 0; i < max; i++)
	{
		if(ImGui::TreeNode(("Horse_" + std::to_string(i)).c_str()))
		{
			horses[i]->ImGuiUpdate();
			ImGui::TreePop();
		}
	}
	ImGui::End();
#endif

	GoalHorseNumUpdate();
}

HorsePlayer* HorseManager::GetPlayer() const
{
	return player;
}

void HorseManager::RaceStart()
{
	for (auto& h : horses)
		h->RaceStart();
}

Horse* HorseManager::GetHorse(int _racePosition)
{
	for (const auto& h : horses)
	{
		if (h->GetRacePosition() == _racePosition)
			return h;
	}

	return nullptr;
}

int HorseManager::GetGoalHorseNum() const
{
	return goalHorseNum;
}

std::vector<HorseManager::HorsePosition> HorseManager::RacePositionUpdate()
{
	//リストへの追加・ソート
	std::vector<HorsePosition> horsePositions;
	for (int i=0;i<horses.size();i++)
	{
		float dis = horses[i]->GetRemainingDistance();
		HorsePosition pos = HorsePosition(horses[i], i, dis);
		horsePositions.push_back(pos);
	}

	std::sort(horsePositions.begin(), horsePositions.end());

	//順番に基づいて順位を指定
	for (int i = 0; i < horsePositions.size(); i++)
	{
		//ゴール後の馬は順位をもう変更しない
		if (horsePositions[i].horse->GetIsGoal())
			continue;

		horsePositions[i].horse->RacePositionUpdate(i, (float)(i) / (horsePositions.size() - 1));
	}

	return horsePositions;
}

void HorseManager::GoalHorseNumUpdate()
{
	//ゴールした馬の数を数える
	for (; goalHorseNum < horses.size(); goalHorseNum++)
	{
		const auto& h = GetHorse(goalHorseNum);

		//ゴールしていないならループから抜ける
		if (not h->GetIsGoal())
			return;
	}
}
