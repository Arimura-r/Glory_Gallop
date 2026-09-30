#pragma once
#include "../Library/GameObject.h"

class HorseManager;
class Horse;
class HorsePlayer;
class CourseDataManager;
class CommonManager;
class InputManager;
class SoundManager;
class GameManager;

class WatchCamera;

class Camera :public GameObject
{
public:
	Camera(HorseManager* _horse,CourseDataManager* _course,GameManager* _gameManager,CommonManager* _common,bool _canPlayerControll);
	~Camera();
	void Update()override;

	Horse* GetCameraTargetHorse()const { return cameraTargetHorse; }
	VECTOR3 GetCameraPosition()const { return cameraPosition; }

	static const float DEFAULT_CAMERA_MOVE_SPEED_MAX;//1秒間に動くカメラの最大距離

private:
	enum CameraState
	{
		ALL_OVERLOOK=0,//全体見下ろし
		FIRST_OVERLOOK,//先頭の馬を見下ろす
		PLAYER_FOLLOW,//プレイヤーを後ろから追うカメラ
		PLAYER_OVERLOOK,//プレイヤーの付近を見下ろし
		HORSE_FOLLOW,//各馬を選んで追従するカメラ
		HORSE_OVERLOOK,//各馬を選んで見下ろすカメラ
		RACE_POSITION_OVERLOOK,//特定の順位の馬を選んで見下ろすカメラ
		WATCH_CAMERA,//テレビ中継のように横から見る観戦用カメラ
		FREE_OVERLOOK,//フリーの見下ろしカメラ

		STATE_NUM,
	};

	void ImGuiUpdate();

	void AllOverlookCameraUpdate();
	void FirstOverlookCameraUpdate();
	void PlayerFollowCameraUpdate();
	void PlayerOverlookCameraUpdate();
	void HorseFollowCameraUpdate();
	void HorseOverlookCameraUpdate();
	void RacePositionOverlookCameraUpdate();
	void FreeOverlookCameraUpdate();

	//cameraPositionとtargetを実際のカメラの座標に適用する
	void CameraPositionApply();
	//補間なしでPosition,Targetの値を変更する
	void CameraTypeChange();

	CameraState prevState;
	CameraState currentState;
	CameraState nextState;
	float cameraMoveSpeed;

	HorseManager* horseManager;
	HorsePlayer* player;
	CourseDataManager* course;
	GameManager* gameManager;
	InputManager* input;
	SoundManager* sound;

	WatchCamera* watchCamera;

	//前フレームのカメラの情報
	VECTOR3 lastCameraPosition;
	VECTOR3 lastCameraTarget;
	//現フレームのカメラの情報
	VECTOR3 cameraPosition;
	VECTOR3 cameraTarget;

	Horse* cameraTargetHorse;
	//FreeCamera用の変数　ToDoこのクラスが分厚くなるならクラスごとに処理を分ける
	VECTOR3 freeOverlookCameraPosition;
	VECTOR3 freeOverlookCameraTarget;
	//HorseCamera用の変数
	int followHorseIndex;//追従する馬の要素番号
	int followRacePosition;
	//PlayerOverlookCamera用の変数
	float overlookCameraHeight;
};