#pragma once
#include "Horse.h"
#include <vector>

class HorseComputer :public Horse
{
public:
	HorseComputer(VECTOR3 _pos, int _horseNumber, int _horseNum,int _model, COLOR_F  _color, const HorseParam& _param, CourseDataManager* _course, Animator* _anim, CommonManager* _common);
	~HorseComputer();
	void Update()override;
	void ImGuiUpdate()override;

	void PrevPositionHorseCheck(Horse* _horse) { prevHorse = _horse; }
	void BehindPositionHorseCheck(Horse* _horse) { behindHorse = _horse; }

	void FrontHorseAdd(Horse* _horse)override;
	void InnerHorseAdd(Horse* _horse)override;

	//追い抜きに入るかどうかの処理
	void OvertakeCheckUpdate();
	//追い抜きの処理
	void OvertakeUpdate();

private:
	const float SPURT_RAND_RANGE = 0.15f;
	const float MOVE_IN_CORNER_LOSS = 300.0f;//これより距離ロスが多くなったら内に移動を狙う
	const float MOVE_IN_INNER_DISTANCE = 10.0f;//内側のラインとの距離がこれ以下なら内に移動しない
	const float CAN_OVERTAKE_SPEED = 1000.0f;//これ以上速かったら追い抜きをする

	const float BEHIND_HORSE_CHECK_DISATANCE = 5000.0f;//後ろの馬との距離がこれ以内なら後ろの馬のことを警戒する
	const float SPEED_UP_BEHIND_HORSE_SPEED_DIF = 30.0f;//後ろの馬との速度がこれ以内なら速度を上げる

	//std::list<int>whipPos;
	Horse* prevHorse;//自分の1つ前の順位の馬の情報
	Horse* behindHorse;//自分の1つ前の順位の馬の情報
	float targetSpeed;
	std::vector<Horse*> frontHorses;//自分の前方の馬
	std::vector<Horse*> innerHorses;//自分の内側の馬
	bool overtake;//追い抜き中かどうか
	float whipDistance;//ここを過ぎたら鞭を入れ始める
	bool inSpurt;//スパート中かどうか
	float spurtCalcRand;//スパートタイミングをバラつかせるための乱数

	//int targetPosition;//道中でこれより前の順位を目指す

	//中盤(スタート後～スパート前)のAI処理
	void MiddleRunAIUpdate();
	//鞭を入れる距離を計算する
	float CalcWhipDistance()const;
	//残り距離的にスパートが出来るかどうかを計算する
	void CalcSpurt();
	//スタート時の処理
	void StartUpdate();
	
};