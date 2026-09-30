#pragma once
#include "../Library/GameObject.h"
#include <vector>

class Horse;
class CourseDataManager;
class HorsePlayer;
class HorseAnimLoader;
class HorseModelLoader;
class HorseCollisionManager;
class HorseCommentData;
class CommonManager;
class Stage;

class HorseManager :public GameObject
{
public:
	HorseManager(CourseDataManager* _course,CommonManager* _common,Stage* _stage,bool _canPlayerControll);
	~HorseManager();
	void Update()override;
	const std::vector<Horse*>& GetHorses()const { return horses; }
	//プレイヤーのクラスを入手する
	HorsePlayer* GetPlayer()const;
	//レースを始める時に呼ぶ
	void RaceStart();
	//特定の順位の馬を得る(1位=0)
	Horse* GetHorse(int _racePosition);
	//ゴールした馬の数を得る
	int GetGoalHorseNum()const;

	HorseCommentData* GetHorseCommentData()const { return comment; }

private:
	struct HorsePosition
	{
		Horse* horse;
		int number;//枠番
		float remainingDistance;//残り距離

		HorsePosition(Horse* _horse, int _num, float _dis)
		{
			horse = _horse;
			number = _num;
			remainingDistance = _dis;
		}

		bool operator <(HorsePosition _right)const
		{
			//残り距離でソートする
			if (remainingDistance != _right.remainingDistance)
				return remainingDistance < _right.remainingDistance;

			//距離が同じなら番号が内の方を前にする ToDoより正確な順位付けが必要であれば修正する
			return number < _right.number;
		}
	};
	//現在順位の更新
	std::vector<HorsePosition> RacePositionUpdate();
	//ゴールした馬の数を数える
	void GoalHorseNumUpdate();

	//各馬の色(デバッグ用)
	const std::vector<COLOR_F>colors
	{
		COLOR_F{1.0f,1.0f,1.0f,1.0f},
		COLOR_F{1.0f,1.0f,1.0f,1.0f},
		COLOR_F{1.0f,1.0f,1.0f,1.0f},
		COLOR_F{1.0f,1.0f,1.0f,1.0f},
		COLOR_F{1.0f,1.0f,1.0f,1.0f},
		COLOR_F{1.0f,1.0f,1.0f,1.0f},
		COLOR_F{1.0f,1.0f,1.0f,1.0f},
		COLOR_F{1.0f,1.0f,1.0f,1.0f},
		COLOR_F{1.0f,1.0f,1.0f,1.0f},
		COLOR_F{1.0f,1.0f,1.0f,1.0f},
		COLOR_F{1.0f,1.0f,1.0f,1.0f},
		COLOR_F{1.0f,1.0f,1.0f,1.0f},
		COLOR_F{0.5f,0.5f,0.5f,1.0f},
		COLOR_F{0.5f,0.5f,0.5f,1.0f},
		COLOR_F{0.5f,0.5f,0.5f,1.0f},
		COLOR_F{0.5f,0.5f,0.5f,1.0f},
		COLOR_F{0.5f,0.5f,0.5f,1.0f},
		COLOR_F{0.5f,0.5f,0.5f,1.0f},
	};
	//const int HORSE_NUM = 8;
	std::vector<Horse*> horses;
	HorsePlayer* player;//プレイヤーへのポインタ(リストと両方に持たせる)
	CourseDataManager* course;
	HorseAnimLoader* anim;
	HorseModelLoader* model;
	HorseCollisionManager* collision;
	HorseCommentData* comment;

	int goalHorseNum;//ゴールした馬の数
};