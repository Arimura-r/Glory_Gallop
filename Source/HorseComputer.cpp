#include "HorseComputer.h"
#include "CourseDataManager.h"
#include "../Library/Random.h"

HorseComputer::HorseComputer(VECTOR3 _pos, int _horseNumber, int _horseNum, int _model, COLOR_F  _color, const HorseParam& _param, CourseDataManager* _course, Animator* _anim, CommonManager* _common) :Horse(_pos, _horseNumber,_horseNum, _model,_color, _param, _course, _anim, _common)
{
	overtake = false;
	prevHorse = nullptr;
	behindHorse = nullptr;
	targetSpeed = param.speed.baseSpeed;
	whipDistance = CalcWhipDistance();
	spurtCalcRand = Random::GetFloat(1.0f, SPURT_RAND_RANGE);//スパートを掛けるタイミングを若干バラつかせる
}

HorseComputer::~HorseComputer()
{
}

void HorseComputer::Update()
{
	Horse::Update();
	if (not outGate)
		return;

	CalcSpurt();
	if (inSpurt)
	{
		targetSpeed=param.speed.topSpeed;
	}
	else
	{
		if (param.speed.currentSpeed < param.speed.baseSpeed &&
			param.speed.currentTargetSpeed >= param.speed.baseSpeed)
		{
			StartUpdate();
		}
		else
		{
			MiddleRunAIUpdate();
		}
	}

	OvertakeCheckUpdate();

	if (overtake)
	{
		OvertakeUpdate();
		//MV1SetDifColorScale(hModel, COLOR_F{ 1.0f,0.0f, 0.0f,1.0f });
	}
	else
	{
		float cornerLoss = course->GetCornerLossDistance(position, currentSegment);
		if (cornerLoss > MOVE_IN_CORNER_LOSS&&course->GetInnerToDistance(position,(CourseDataManager::SegmentType)currentSegment)<MOVE_IN_INNER_DISTANCE)
		{
			MoveIn();
		}
	}

	targetSpeed = max(param.speed.minSpeed, targetSpeed);
	targetSpeed = min(param.speed.topSpeed, targetSpeed);

	//目標速度に基づいて速度調整を行う
	if(not whipping)
	{
		if (targetSpeed > param.speed.currentTargetSpeed)
		{
			SpeedUp();
		}
		else if (targetSpeed < param.speed.currentTargetSpeed)
		{
			SpeedDown();
		}
	}


	//	//ToDo　馬同士の当たり判定なども入れたら、内への入りやすさ、その後の動きやすさなども調べる

	//スタミナがなくなったら鞭を入れる
	if (param.startOutEnergy)
	{
		Whip();
	}

	frontHorses.clear();
	innerHorses.clear();

	/*if (param.speed.currentSpeed == param.speed.currentTargetSpeed)
		AutoDecel();*/
}

void HorseComputer::ImGuiUpdate()
{
	Horse::ImGuiUpdate();
	//ImGui::Text("TargetPos=%d", param.targetPosition);
	/*if (ImGui::TreeNode("WhipPos"))
	{
		for (const auto& w : whipPos)
		{
			ImGui::Text("%d", w);
		}
		ImGui::TreePop();
	}*/
}

void HorseComputer::FrontHorseAdd(Horse* _horse)
{
	frontHorses.push_back(_horse);
}

void HorseComputer::InnerHorseAdd(Horse* _horse)
{
	innerHorses.push_back(_horse);
}

void HorseComputer::OvertakeCheckUpdate()
{
	//前に馬がいるなら
	if (frontHorses.size() > 0)
	{
		if (param.speed.currentSpeed > CAN_OVERTAKE_SPEED)
		{
			for (const auto& h : frontHorses)
			{
				//前の馬が自分より遅いなら追い抜きモードに入る
				if (h->GetSpeedParam().currentSpeed < param.speed.currentSpeed)
				{
					overtake = true;
					break;
				}
			}
		}
	}
}

void HorseComputer::OvertakeUpdate()
{
	//前に馬がいるなら外に行く
	if (frontHorses.size() > 0)
	{
		MoveOut();
	}
	else
	{
		//内側に馬がいないなら追い抜きを終える
		if (innerHorses.size() < 1)
			overtake = false;
	}
}

void HorseComputer::MiddleRunAIUpdate()
{
	//目標速度まで達しており、現在のペースでスタミナに余裕があるなら
	/*if (param.speed.currentSpeed>=targetSpeed&&GetCurrentStaminaPace()>0)
	{
		targetSpeed += 10.0f;
	}*/

	if (param.speed.currentSpeed != targetSpeed)
		return;

	//目標順位に達しているかを確認する
	if (param.chargingBoost)
	{
		//抜かれたら脚を溜められなくなる場合、後ろの馬を確認する
		if ((float)(racePosition + 1)/allHorseNum>param.boost.backChargeRange)
		{
			//ToDo　速度を上げる条件と速度を落とす条件を分ける

			//後ろの馬が自分より速く、距離も近いなら速度を上げる
			if (behindHorse !=nullptr&&
				behindHorse->GetRemainingDistance() - remainingDistance<BEHIND_HORSE_CHECK_DISATANCE &&
				behindHorse->GetParam().speed.currentSpeed>max(param.speed.currentSpeed,param.speed.currentTargetSpeed)+SPEED_UP_BEHIND_HORSE_SPEED_DIF)
				targetSpeed += TARGET_SPEED_UP_NUM;
			else
			{
				if (targetSpeed > param.speed.baseSpeed)
					targetSpeed -=SPEED_DOWN_FRAME_SPEED;
			}
		}
		else
		{
			if (targetSpeed > param.speed.baseSpeed)
				targetSpeed -= SPEED_DOWN_FRAME_SPEED;
		}
	}
	else
	{
		//最高順位よりも前にいる場合は速度を落とす
		if (param.boost.frontChargeRange > racePositionRate)
		{
			if (targetSpeed > param.speed.baseSpeed)
				targetSpeed -= SPEED_DOWN_FRAME_SPEED;
		}
		//最低順位よりも後ろにいる場合は速度を上げる
		else if (param.boost.backChargeRange < racePositionRate&&
			max(param.speed.currentTargetSpeed,param.speed.currentSpeed)<=prevHorse->GetParam().speed.currentSpeed+50.0f&&
			param.stamina.GetCurrentStaminaRate()>0.3f)
		{
			targetSpeed += TARGET_SPEED_UP_NUM;
		}
	}
}

void HorseComputer::CalcSpurt()
{
	float maxStaminaRunTime = (param.stamina.currentStamina+param.boost.chargeBoost) / param.cost.maxSpeedCost;//最高速度で走れる時間

	//最高速度で走れる距離が残り距離よりも短かったら最高速度まで一気に加速する																				現在座標から馬2頭分外を回される計算						
	if ((param.speed.topSpeed - (param.speed.topSpeed - param.speed.currentSpeed) / param.speed.currentAccelaration) * maxStaminaRunTime * spurtCalcRand > remainingDistance - whipDistance + course->GetCornerLossDistance(position, currentSegment, collisionRadius * 2.0f))
		inSpurt = true;
}

void HorseComputer::StartUpdate()
{
	if (racePositionRate > param.boost.backChargeRange)
	{
		StartSpeedUp();
	}
}

float HorseComputer::CalcWhipDistance()const
{
	return 	param.speed.topSpeed * param.whipNum * param.whipSpeedUpTime * Random::GetFloat(1.0f, SPURT_RAND_RANGE);//乱数で少しスパート時のタイミングをずらす
}
