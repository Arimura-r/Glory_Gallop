#pragma once
#include "../Library/GameObject.h"

class CourseDataManager;
class HorseManager;

//観戦用カメラクラス
class WatchCamera :public GameObject
{
public:
	WatchCamera(CourseDataManager* _course,HorseManager* _horse);
	~WatchCamera();
	void Update();
	void CameraUpdate(VECTOR3& _cameraPosition,VECTOR3& _cameraTarget);

	//カメラの位置を強制的に変える(補間なしでPosition,Targetを変更する)リクエストを受け取る(Camera側で使用)
	bool GetCameraChangeRequest()const;
	//カメラの移動速度の変更のリクエストを受け取る
	float GetCameraMoveSpeedRequest()const;

private:
	enum Segment
	{
		START,//スタート直後
		BACK_STREATCH_SIDE,//向こう正面で馬を横から見る
		SECOND_CORNER_BEGIN,//コーナーの入り始め
		SECOND_CORNER_MIDDLE,//コーナーの中間点
		FINAL_CORNER,//最終コーナー
		HOME_STREATCH,//最終直線
		GOAL,//ゴール前
		AFTER_GOAL,//ゴール後
	};

	//スタート時のカメラの挙動　カメラのステートが変わる際にtrueを返す
	void StartUpdate(VECTOR3& _cameraPosition, VECTOR3& _cameraTarget);
	//向こう正面で横から見るカメラの挙動
	void BackStreatchSideUpdate(VECTOR3& _cameraPosition, VECTOR3& _cameraTarget);
	void SecondCornerBeginUpdate(VECTOR3& _cameraPosition, VECTOR3& _cameraTarget);
	void SecondCornerMiddleUpdate(VECTOR3& _cameraPosition, VECTOR3& _cameraTarget);
	void FinalCornerUpdate(VECTOR3& _cameraPosition, VECTOR3& _cameraTarget);
	//最終直線でのカメラの挙動
	void HomeStreatchUpdate(VECTOR3& _cameraPosition, VECTOR3& _cameraTarget);
	//ゴール前のカメラの挙動
	void GoalUpdate(VECTOR3& _cameraPosition, VECTOR3& _cameraTarget);
	//全馬の真ん中の座標を得る
	VECTOR3 GetAllHorseMiddlePosition();

	int targetPositionHorse;//注目する順位の馬
	float horseCloseUpTimer;//各馬に注目する時間のタイマー
	bool cameraChangeRequest;
	float cameraMoveSpeed;

	VECTOR3 targetPositionOffset;

	Segment nextSegment;//区間を変更したい時に利用　区間変化を分かりやすくするため
	Segment currentSegment;//現在の区間　Update以外で変更しない
	Segment prevSegment;//1フレーム前の区間　Update以外で変更しない

	CourseDataManager* course;
	HorseManager* horseManager;
};