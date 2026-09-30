#pragma once
#include "../Library/GameObject.h"

enum GameState
{
	BEFORE_RACE = 0,//レースの開始前
	BEFORE_PLAY,//ゲートイン～スタートまで
	IN_PLAY,//レース中
	RESULT,//レース後
};

class HorseManager;
class Horse;
class CommonManager;
class InputManager;
class SoundManager;
class TicketManager;

class BeforeRaceState;
class ResultState;

//ゲームの進行状況を管理するクラス
class GameManager :public GameObject
{
public:
	GameManager(HorseManager* _horse, CommonManager* _common, TicketManager* _ticket);
	~GameManager();
	void Update()override;

	float GetRaceStartTime()const { return raceStartTime; }
	float GetRaceTime()const { return raceTime; }
	const GameState& GetGameState()const { return state; }

	//カメラ変更用関数
	bool GetChangeAllViewToPlayCamera()const { return changeAllViewToPlayCamera; }

	BeforeRaceState* GetBeforeRaceState()const { return beforeRace; }
	ResultState* GetResultState()const { return result; }

private:
	void BeforePlayUpdate();
	void InPlayUpdate();

	float raceStartTime;//レーススタートまでの時間
	float raceTime;//レース内の経過時間
	const float COUNT_PLAY_TIME=3.0f;//カウントが何秒以下ならカウントSEを流すか
	GameState state;

	HorseManager* horse;
	Horse* player;
	TicketManager* ticket;
	InputManager* input;
	SoundManager* sound;

	BeforeRaceState* beforeRace;
	ResultState* result;

	//カメラ変更用変数
	bool changeAllViewToPlayCamera;
};