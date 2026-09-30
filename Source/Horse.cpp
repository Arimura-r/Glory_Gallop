#include "Horse.h"
#include "../Library/Random.h"
#include "CourseDataManager.h"
#include "Animator.h"
#include "HorseAnimDefine.h"
#include "CommonManager.h"
#include "SoundManager.h"

Horse::Horse(VECTOR3 _pos, int _horseNumber, int _horseNum,int _model, COLOR_F _color, const HorseParam& _param, CourseDataManager* _course, Animator* _anim, CommonManager* _common)
{
	course = _course;
	param = _param;
	hModel = _model;
	position = _pos;
	horseNumber = _horseNumber;
	anim = _anim;
	common = _common;
	color = _color;
	allHorseNum = _horseNum;

	sound = _common->FindCommonObject<SoundManager>();

	//ParamRandomize();

	anim->SetModel(_model);
	arriveGoal = false;
	collisionRadius = DEFUALT_COLLISION_RADIUS;
	racePosition = 1;
	racePositionRate = 0.0f;
	whipping = false;
	whipSpeedUpTimer = 0.0f;
	moveVec = VECTOR3(0, 0, 1);
	rotation = VECTOR3(0, DX_PI_F, 0);
	scale = VECTOR3(0.6f, 0.6f, 0.6f);
	anim->Play(HorseAnim::WAIT);
	speedUpTimer = 0.0f;

	MV1SetFrameUserLocalMatrix(hModel, 1, MGetRotY(180.0f * DegToRad));//キャラクターの向きを反転させる

	inCourse = true;
	inRace = false;

	remainingDistance = _course->GetCourseAllDistance();
	currentSegment = 0;
	outGate = false;
	inGateTimer = param.startSpeed * Random::GetFloat(1.0f, 0.1f);//少し値をブレさせる

	MV1SetDifColorScale(hModel, color);
	AutoCurve();

	//int gl = LoadGraph("data\\model\\character\\texture\\Horse_Body_WhiteBlack.png");
	//MV1SetTextureGraphHandle(hModel, 0, gl, true);
}

Horse::~Horse()
{
	MV1DeleteModel(hModel);
	hModel = -1;
	delete anim;
}

void Horse::Update()
{
	anim->Update();
	Object3D::Update();
	remainingDistance = course->HorseRemainingDistanceUpdate(position, currentSegment);

	//速度に応じて速度を変える
	if (anim->GetCurrentID() == HorseAnim::RUN)
		anim->SetCurrentAnimSpeedRate(max(param.speed.currentSpeed / param.speed.baseSpeed,MIN_RUN_SPEED_RATE));

	if (not inRace)
		return;

	if (not outGate)
	{
		GateUpdate();
		return;
	}

	if (whipping)
	{
		whipSpeedUpTimer -= Time::DeltaTime();
		if (whipSpeedUpTimer < 0)
		{
			whipping = false;
			whipSpeedUpTimer = 0.0f;
			param.speed.WhipEnd();
		}
	}

	param.Update(whipping,racePositionRate);
	velocity = CalcVelocity(moveVec) * GetRotationMatrix();

	//ベクトルの初期化
	moveVec = VECTOR3(0, 0, 1);

	if (speedUpTimer > 0)
	{
		speedUpTimer -= Time::DeltaTime();
		if (speedUpTimer < 0)
			speedUpTimer = 0.0f;
	}
}

void Horse::PositionUpdate()
{
	//velocityの加算
	position += velocity;

	AutoCurve();
}

void Horse::RacePositionUpdate(int _pos, float _rate)
{
	racePosition = _pos;
	racePositionRate = _rate;
}

void Horse::ImGuiUpdate()
{
	ImGui::InputInt("RacePosition", &racePosition);
	ImGui::Text("Name：%s", param.name);
	ImGui::InputFloat3("Position", &position.x);
	ImGui::InputFloat3("Rotation", &rotation.x);
	ImGui::InputInt("WhipNum", &param.whipNum);
	param.speed.ImGuiUpdate();
	param.stamina.ImGuiUpdate();
	param.boost.ImGuiUpdate();
	param.condition.ImGuiUpdate();
}

void Horse::Draw()
{
	Object3D::Draw();

	return;

	//デバッグ表示
	int drawColor = 0xffffff;

	DrawSphere3D(position, collisionRadius, 3, drawColor, 0xffffff, false);

	float hitRadius = DEFUALT_COLLISION_RADIUS*2;
	VECTOR3 frontPos = GetPosition() + VECTOR3(0, 0, 1) * GetRotationMatrix() * hitRadius * 1.5f;
	VECTOR3 innerPos = GetPosition() + VECTOR3(1, 0, 0) * GetRotationMatrix() * hitRadius;
	VECTOR3 innerPos2 = GetPosition() + VECTOR3(1, 0, 0) * GetRotationMatrix() * hitRadius + VECTOR3(0, 0, 1) * GetRotationMatrix() * hitRadius*1.5f;
	VECTOR3 innerPos3 = GetPosition() + VECTOR3(1, 0, 0) * GetRotationMatrix() * hitRadius + VECTOR3(0, 0, -1) * GetRotationMatrix() * hitRadius * 1.5f;

	DrawSphere3D(frontPos, hitRadius, 3, 0xff0000, 0xff0000, false);
	DrawSphere3D(innerPos, hitRadius, 3, 0x00ff00, 0x00ff00, false);
	DrawSphere3D(innerPos2, hitRadius, 3, 0x00ff00, 0x00ff00, false);
	DrawSphere3D(innerPos3, hitRadius, 3, 0x00ff00, 0x00ff00, false);
}

void Horse::GoalIn()
{
	arriveGoal = true;
}

float Horse::GetReamingTime() const
{
	return remainingDistance / param.speed.currentSpeed;
}

void Horse::PositionPush(VECTOR3 _pushVec)
{
	position += _pushVec;
}

void Horse::CheckColVeclocity(VECTOR3 _vel)
{
	velocity = _vel;
}

void Horse::HitOtherHorse()
{
	//param.speed.currentSpeed *= 0.9f;
}

void Horse::CourseOut()
{
	inCourse = false;
}

float Horse::GetCurrentStaminaPace()
{
	const float& lossStamina = GetLossStamina();//1秒間に失うスタミナ
	float remainingTime = GetReamingTime();//ゴールまでの残り時間

	return param.stamina.GetCurrentStamina()-lossStamina * remainingTime;
}

void Horse::RaceStart()
{
	inRace = true;
	param.speed.Start();
	anim->Play(HorseAnim::RUN);
}

void Horse::AutoCurve()
{
	VECTOR3 nextPosition = position + velocity;//次フレームの座標の取得

	//コースデータから次の地点のコース形状を取得
	CourseDataManager::SegmentType type=course->GetCurrentSegmentType(nextPosition);
	float rate = course->GetSegmentRate(nextPosition, type);//現在の区間を何割進んでいるかを取得する
	float startAngle = 0.0f;

	//コーナー以外なら処理を飛ばす
	switch (type)
	{
	case CourseDataManager::HOME_STREATCH:
		rotation.y = 0.0f;
		break;
	case CourseDataManager::BACK_STREATCH:
		rotation.y = DX_PI_F;
		break;
	case CourseDataManager::FIRST_CORNER:
	case CourseDataManager::SECOND_CORNER:
		//左側のコーナーは直線からの角度がDX_PI_Fから始まる
		if (type == CourseDataManager::SECOND_CORNER)
			startAngle = DX_PI_F;

		rotation.y = rate * DX_PI_F + startAngle;
		break;
	default:
		break;
	}
}

VECTOR3 Horse::CalcVelocity(VECTOR3 _moveVec)
{
	VECTOR3 vel = _moveVec * Time::DeltaTime();
	//vel *= course->GetData().GetCourseSpeed(param.speed.currentSpeed, position);//前方向に進む際のスピードにのみコーナー分の減速を掛ける
	vel *= param.speed.currentSpeed;
	return vel;
}

bool Horse::Whip()
{
	if (whipping||param.whipNum<=0)
		return false;

	param.speed.Whip();
	//param.speed.currentSpeed *= 1.1f;
	whipping = true;
	whipSpeedUpTimer = param.whipSpeedUpTime;
	param.whipNum--;

	return true;
}

void Horse::MoveIn()
{
	moveVec = MOVE_LANE_IN_VEC;
	//それ以上内に行けなくなったらその分の移動量を前方向に変換する
	/*float posX = position.x + moveVec.x;
	if (posX < course->GetData().minX)
	{
		moveVec.x -= posX - course->GetData().minX;
		moveVec=moveVec.Normalize();
	}*/
}

void Horse::MoveOut()
{
	moveVec = MOVE_LANE_OUT_VEC;
	////それ以上外に行けなくなったらその分の移動量を前方向に変換する
	//float posX = position.x + moveVec.x;
	//if (posX > course->GetData().maxX)
	//{
	//	moveVec.x -= posX- course->GetData().maxX;
	//	moveVec = moveVec.Normalize();
	//}
}

void Horse::TurnLeft(float _rotChangeSpeed)
{
	rotation.y -= _rotChangeSpeed;
}

void Horse::TurnRight(float _rotChangeSpeed)
{
	rotation.y += _rotChangeSpeed;
}

void Horse::StartSpeedUp()
{
	if (param.stamina.GetCurrentStaminaRate() > 0.0f ||
		param.boost.GetChargeBoostRate() > 0.0f)
	{
		if(speedUpTimer<=0.0f)
		{
			speedUpTimer = SPEED_UP_INTERVAL;
			//スタートなどの場合、現在速度と加速度を少し上げる
			param.speed.currentSpeed += 1.5f;
			param.speed.currentAccelaration += 5.0f;
			param.stamina.currentStamina -= 0.3f;
		}
	}
}

void Horse::SpeedUp()
{
	if (param.stamina.GetCurrentStaminaRate() > 0.0f ||
		param.boost.GetChargeBoostRate() > 0.0f)
	{
		if (speedUpTimer <= 0.0f)
		{
			speedUpTimer = SPEED_UP_INTERVAL;
			//目標速度を上げてそこに向けて加速を開始する
			param.speed.Accelaration(max(param.speed.currentTargetSpeed, param.speed.currentSpeed) + TARGET_SPEED_UP_NUM);
		}
	}
}

void Horse::SpeedDown()
{
	//param.speed.currentSpeed -= 25.0f;
	//param.speed.currentTargetSpeed -= 25.0f;
	param.speed.SpeedDown(SPEED_DOWN_FRAME_SPEED);
}

void Horse::ParamRandomize()
{
	//param.speed.baseSpeed *= Random::GetFloat(1.0f, BASE_SPEED_RAND_RANGE);
	param.speed.topSpeed *= Random::GetFloat(1.0f, TOP_SPEED_RAND_RANGE);
	//param.speed.startAccelaration *= Random::GetFloat(1.0f, START_ACCE_RAND_RANGE);

	//param.stamina.minSpeedCost *= Random::GetFloat(1.0f, 0.10f);
	//param.stamina.baseSpeedCost *= Random::GetFloat(1.0f, 0.10f);
	param.cost.maxSpeedCost *= Random::GetFloat(1.0f, MAX_SPEED_STAMINA_CONST_RAND_RANGE);
	param.cost.whipCost *= Random::GetFloat(1.0f, WHIP_STAMINA_CONST_RAND_RANGE);

	param.whipSpeedUpTime *= Random::GetFloat(1.0f, WHIP_SPEED_UP_TIME_RAND_RANGE);
}

void Horse::GateUpdate()
{
	inGateTimer -= Time::DeltaTime();
	if (inGateTimer < 0.0f)
	{
		inGateTimer = 0.0f;
		outGate = true;
	}
}

void Horse::AutoDecel()
{
	param.speed.Decelaration(param.speed.baseSpeed);
}
