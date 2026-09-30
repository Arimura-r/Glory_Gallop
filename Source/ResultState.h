#pragma once
#include "../Library/GameObject.h"

class InputManager;
class SoundManager;
class HorseManager;
class TicketManager;

class ResultState :public GameObject
{
public:
	//何を現在描画するかの状態管理
	enum ResultDrawState
	{
		NONE=-1,
		GOAL_WAIT=0,//ゴール待ち状態
		RACE_POSITION,//全着順表示
		PAY_OUT_RESULT,//各馬券の払い戻し金の表示
		TICKET_HIT,//購入馬券が当たったかどうかの確認

		PLAYER_GOAL_IN,//プレイヤー操作モードでプレイヤーがゴール後の状態
	};

	ResultState(InputManager* _input, SoundManager* _sound,HorseManager* _horse,TicketManager* _ticket);
	~ResultState();
	void StateUpdate();

	ResultDrawState GetCurrentDrawState()const { return state; }

private:
	void GoalWaitStateUpdate();
	void RacePositionStateUpdate();
	void PayOutResultStateUpdate();
	void TicketHitStateUpdate();
	void PlayerGoalInUpdate();

	void TitleBackUpdate();

	InputManager* input;
	SoundManager* sound;
	HorseManager* horseManager;
	TicketManager* ticketManager;

	ResultDrawState state;
	bool titleBackFlg;
};