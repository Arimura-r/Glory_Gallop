#pragma once
#include "../Library/Object3D.h"
#include "../ImGui/imgui.h"
#include "HorseParam.h"

class CourseDataManager;
class Animator;
class CommonManager;
class SoundManager;

class Horse :public Object3D
{
public:
	Horse(VECTOR3 _pos,int _horseNumber,int _horseNum,int _model, COLOR_F _color,const HorseParam& _param,CourseDataManager* _course,Animator* _anim, CommonManager* _common);
	virtual ~Horse();
	virtual void Update()override;
	//実際の移動の処理(velocityからpositionの加算の処理のみ)
	void PositionUpdate();
	void RacePositionUpdate(int _pos,float _rate);
	virtual void ImGuiUpdate();
	virtual void Draw()override;

	virtual void GoalIn();
	bool GetIsGoal()const { return arriveGoal; }
	float GetCollisionRadius()const { return collisionRadius; };
	VECTOR3 GetVelocity()const { return velocity; }
	HorseParam GetParam()const { return param; }
	int GetRacePosition()const { return racePosition; }
	float GetRemainingDistance()const { return remainingDistance; }
	SpeedParam GetSpeedParam()const { return param.speed; }
	StaminaParam GetStaminaParam()const { return param.stamina; }
	bool GetWhipping()const { return whipping; }
	int GetHorseNumber()const { return horseNumber; }
	//1秒間に失うスタミナ消費量を得る
	float GetLossStamina()const { return param.cost.LossCost(whipping,param.speed); }
	//現在速度で走った時のゴールまでの時間を得る
	float GetReamingTime()const;

	//スタートの合図が切られたときに呼ぶ関数
	void RaceStart();
	//当たり判定などの押し返し
	void PositionPush(VECTOR3 _pushVec);
	//衝突回避のためにVelocityを調整する
	void CheckColVeclocity(VECTOR3 _vel);
	//他馬と当たった時の処理
	void HitOtherHorse();
	//コースアウト中は毎フレーム呼ぶ
	void CourseOut();
	//前方にいる馬の情報を加える
	virtual void FrontHorseAdd(Horse* _horse) {};
	//内側にいる馬の情報を加える
	virtual void InnerHorseAdd(Horse* _horse) {};

	//自分の前のポジションの馬の情報を得る
	virtual void PrevPositionHorseCheck(Horse* _horse) {};
	//自分の後ろのポジションの馬の情報を得る
	virtual void BehindPositionHorseCheck(Horse* _horse) {};

	//現在のスタミナの消費の仕方で最後にスタミナがどれだけ残るか
	float GetCurrentStaminaPace();

	const float DEFUALT_COLLISION_RADIUS = 70.0f;

protected:
	//const float WHIP_SPEED_UP_TIME_MEAN = 300.0f;//平均値
	//const float WHIP_SPEED_UP_TIME_DEV = 150.0f;//標準偏差
	//const int WHIP_NUM_MIN = 2;
	//const int WHIP_NUM_MAX = 4;
	const VECTOR3 MOVE_LANE_OUT_VEC = VECTOR3(-0.2f,0,0.9f);//外に移動するときの前・横方向へのベクトル
	const VECTOR3 MOVE_LANE_IN_VEC = VECTOR3(0.2f, 0, 0.9f);//内に移動するときの前・横方向へのベクトル
	const float BASE_SPEED_RAND_RANGE = 0.02f;
	const float TOP_SPEED_RAND_RANGE = 0.03f;
	const float START_ACCE_RAND_RANGE = 0.1f;
	const float MAX_SPEED_STAMINA_CONST_RAND_RANGE = 0.02f;
	const float WHIP_STAMINA_CONST_RAND_RANGE = 0.05f;
	const float WHIP_SPEED_UP_TIME_RAND_RANGE = 0.03f;
	const float MIN_RUN_SPEED_RATE = 0.75f;
	const float TARGET_SPEED_UP_NUM = 20.0f;//ボタン1回押すごとに上がる速度の目標値
	const float SPEED_DOWN_FRAME_SPEED = 2.0f;//1フレームにスピードダウンする速度
	const float SPEED_UP_INTERVAL = 0.1f;//StartSpeedUp,SpeedUpを最短で何秒で次に呼べるかどうか
	
	//進行方向がコーナーなら自然に曲がるようにする
	void AutoCurve();
	//速度を計算する
	VECTOR3 CalcVelocity(VECTOR3 _moveVec);

	//鞭を入れる
	bool Whip();
	//内側に移動する
	void MoveIn();
	//外側に移動する
	void MoveOut();
	//左に曲がる
	void TurnLeft(float _rotChangeSpeed);
	//右に曲がる
	void TurnRight(float _rotChangeSpeed);
	//スタート時に追う
	void StartSpeedUp();
	//速度を上げる(追う)
	void SpeedUp();
	//速度を下げる(引っ張る)
	void SpeedDown();
	//パラメータをランダムに少し変える
	void ParamRandomize();
	//スタート後にゲートを出るまでのUpdate
	void GateUpdate();
	//加速をさせていない場合、徐々に通常速度まで速度を落とす
	void AutoDecel();

	VECTOR3 velocity;
	VECTOR3 moveVec;
	//int color;
	bool arriveGoal;
	//SpeedParam speed;
	//int whipNum;//鞭を打てる残り回数
	bool whipping;//鞭を今打たれているかどうか
	float whipSpeedUpTimer;//鞭を打って速度を上げている時のタイマー
	float collisionRadius;
	int racePosition;//順位
	float racePositionRate;//走行順位を0.0~1.0で表示
	float remainingDistance;//残り距離
	int currentSegment;//現在位置している区間
	//float whipSpeedUpTime;
	HorseParam param;
	int horseNumber;//馬番
	bool inRace;//レース中かどうか
	bool outGate;//ゲートを出たかどうか
	float inGateTimer;//ゲート内の経過時間
	int allHorseNum;//レースの全馬の数

	float speedUpTimer;

	COLOR_F color;//馬の色(デバッグ用)

	CourseDataManager* course;
	Animator* anim;
	CommonManager* common;
	SoundManager* sound;

	//デバッグ用の変数
	bool inCourse;//コース内に入っているか
};