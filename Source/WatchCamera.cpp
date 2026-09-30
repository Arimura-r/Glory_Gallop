#include "WatchCamera.h"
#include "CourseDataManager.h"
#include "HorseManager.h"
#include "Horse.h"
#include "Camera.h"
#include "Easing.h"
#include "CommonCameraFunc.h"

namespace
{
	const float START_CAMERA_HEIGHT = 500.0f;
	const float START_CAMERA_STARTLINE_DISTANCE = 5000.0f;//スタート時カメラのスタートラインとの距離
	const float START_CAMERA_TRANSITION_TARGET_DISTANCE = 5000.0f;//ターゲットとの距離がいくつ離れたら次のカメラに移行するか

	const VECTOR3 BACK_STERATCH_CAMERA_TARGET_OFFSET=VECTOR3(1000.0f, 300.0f, 0);//ターゲットからカメラ座標への距離
	const float BACK_STREATCH_CAMERA_MOVE_TO_CENTER_SECOND = 15.0f;//カメラが先頭から中央に移動するまでの時間

	const VECTOR3 SECOND_CORNER_BEGIN_CAMERA_POSITION= VECTOR3(50000, 1000, -37000);
	const float SECOND_CORNER_BEGIN_CAMERA_ZOOMIN_DISTANCE = 3000.0f;
	const float SECOND_CORNER_BEGIN_CAMERA_TRANSITION_REMAINING_DISTANCE = 107500.0f;//先頭の残り距離がこれ以下なら次のカメラに移行する

	const float SECOND_CORNER_MIDDLE_CAMERA_MOVE_TO_CENTER_SECOND = 10.0f;//カメラが先頭から中央に移動するまでの時間
	const VECTOR3 SECOND_CORNER_MIDDLE_CAMERA_TARGET_OFFSET = VECTOR3(0, 500, 1000);//ターゲットからカメラ座標への距離
	const float SECOND_CORNER_MIDDLE_CAMERA_TRANSITION_REMAINING_DISTANCE = 80000.0f;//先頭の残り距離がこれ以下なら次のカメラに移行する

	const VECTOR3 FINAL_CORNER_CAMERA_POSITION = VECTOR3(7000, 2500, -23000);
	const float FINAL_CORNER_CAMERA_ZOOMIN_DISTANCE = 2500.0f;

	const VECTOR3 HOME_STREATCH_CAMERA_TARGET_OFFSET= VECTOR3(0, 2000, 6000);//ターゲットからカメラ座標への距離
	const float HOME_STREATCH_CAMERA_ZOOMIN_DISTANCE = 2000.0f;
}

WatchCamera::WatchCamera(CourseDataManager* _course, HorseManager* _horse):
	course(_course),horseManager(_horse),currentSegment(START), prevSegment(START), nextSegment(START), cameraChangeRequest(false),horseCloseUpTimer(0.0f), 
	targetPositionHorse(0),cameraMoveSpeed(Camera::DEFAULT_CAMERA_MOVE_SPEED_MAX)
{
}

WatchCamera::~WatchCamera()
{
}

void WatchCamera::Update()
{
	//それぞれ次の区間への遷移条件を記載
	switch (currentSegment)
	{
	case WatchCamera::START:
		break;
	case WatchCamera::BACK_STREATCH_SIDE:
		break;
	case WatchCamera::SECOND_CORNER_BEGIN:
		break;
	case WatchCamera::SECOND_CORNER_MIDDLE:
		break;
	case FINAL_CORNER:
		break;
	case WatchCamera::HOME_STREATCH:
		break;
	case WatchCamera::GOAL:
		break;
	case WatchCamera::AFTER_GOAL:
		break;
	default:
		break;
	}
}

void WatchCamera::CameraUpdate(VECTOR3& _cameraPosition, VECTOR3& _cameraTarget)
{
	cameraChangeRequest = false;
	//各区間に応じてUpdateを回す
	switch (currentSegment)
	{
	case WatchCamera::START:
		StartUpdate(_cameraPosition, _cameraTarget);
		break;
	case WatchCamera::BACK_STREATCH_SIDE:
		BackStreatchSideUpdate(_cameraPosition, _cameraTarget);
		break;
	case WatchCamera::SECOND_CORNER_BEGIN:
		SecondCornerBeginUpdate(_cameraPosition,_cameraTarget);
		break;
	case WatchCamera::SECOND_CORNER_MIDDLE:
		SecondCornerMiddleUpdate(_cameraPosition, _cameraTarget);
		break;
	case FINAL_CORNER:
		FinalCornerUpdate(_cameraPosition, _cameraTarget);
		break;
	case WatchCamera::HOME_STREATCH:
		HomeStreatchUpdate(_cameraPosition,_cameraTarget);
		break;
	case WatchCamera::GOAL:
		GoalUpdate(_cameraPosition, _cameraTarget);
		break;
	case WatchCamera::AFTER_GOAL:
		break;
	default:
		break;
	}

	//基本的にはprevをcurrentに更新する処理を回すが、次の区間に変更があった時のみprevの更新を1フレームのみ行わない
	if (nextSegment != currentSegment)
		currentSegment = nextSegment;
	else
		prevSegment = currentSegment;
}

bool WatchCamera::GetCameraChangeRequest()const
{
	return cameraChangeRequest;
}

float WatchCamera::GetCameraMoveSpeedRequest() const
{
	return cameraMoveSpeed;
}

void WatchCamera::StartUpdate(VECTOR3& _cameraPosition, VECTOR3& _cameraTarget)
{
	_cameraPosition = VECTOR3(course->GetRightCornerCenter().x+course->GetInnerLine()+course->GetCourseWidth()/2,
		START_CAMERA_HEIGHT, course->GetStartLineData().innerPos.z - START_CAMERA_STARTLINE_DISTANCE);
	_cameraTarget = GetAllHorseMiddlePosition();
	
	if (_cameraPosition.z-_cameraTarget.z>START_CAMERA_TRANSITION_TARGET_DISTANCE)
	{
		nextSegment = BACK_STREATCH_SIDE;
	}
}

void WatchCamera::BackStreatchSideUpdate(VECTOR3& _cameraPosition, VECTOR3& _cameraTarget)
{
	_cameraTarget = GetAllHorseMiddlePosition();

	//1フレームのみカメラ補間をさせない
	if (prevSegment != currentSegment)
	{
		cameraChangeRequest = true;
		targetPositionOffset = horseManager->GetHorse(0)->GetPosition()-_cameraTarget;//先頭～馬群中央への距離を取る
	}

	horseCloseUpTimer += Time::DeltaTime();
	_cameraTarget += targetPositionOffset - (targetPositionOffset / BACK_STREATCH_CAMERA_MOVE_TO_CENTER_SECOND)*horseCloseUpTimer;
	_cameraPosition = _cameraTarget + BACK_STERATCH_CAMERA_TARGET_OFFSET;

	if (course->GetCurrentSegmentType(_cameraTarget) != CourseDataManager::BACK_STREATCH)
	{
		nextSegment = SECOND_CORNER_BEGIN;
		cameraMoveSpeed = Camera::DEFAULT_CAMERA_MOVE_SPEED_MAX;
		horseCloseUpTimer = 0.0f;
	}
}

void WatchCamera::SecondCornerBeginUpdate(VECTOR3& _cameraPosition, VECTOR3& _cameraTarget)
{
	if (prevSegment != currentSegment)
		cameraChangeRequest = true;

	_cameraPosition = SECOND_CORNER_BEGIN_CAMERA_POSITION;
	_cameraTarget = GetAllHorseMiddlePosition();

	_cameraPosition = CommonCameraFunc::ZoomIn(_cameraPosition, _cameraTarget, SECOND_CORNER_BEGIN_CAMERA_ZOOMIN_DISTANCE);

	if (horseManager->GetHorse(0)->GetRemainingDistance() < SECOND_CORNER_BEGIN_CAMERA_TRANSITION_REMAINING_DISTANCE)
	{
		nextSegment = SECOND_CORNER_MIDDLE;
	}
}

void WatchCamera::SecondCornerMiddleUpdate(VECTOR3& _cameraPosition, VECTOR3& _cameraTarget)
{
	if (prevSegment != currentSegment)
	{
		cameraChangeRequest = true;
	}

	float moveSecond = SECOND_CORNER_MIDDLE_CAMERA_MOVE_TO_CENTER_SECOND;
	//先頭から徐々に中央に寄せていく
	if(horseCloseUpTimer< moveSecond)
	{
		VECTOR3 toFirstVec = horseManager->GetHorse(0)->GetPosition() - GetAllHorseMiddlePosition();
		_cameraTarget = GetAllHorseMiddlePosition() + toFirstVec - (toFirstVec / moveSecond) * horseCloseUpTimer;
	}
	else//中央から最下位に寄せていく
	{
		VECTOR3 toLastVec = horseManager->GetHorse((int)horseManager->GetHorses().size()-1)->GetPosition() - GetAllHorseMiddlePosition();
		_cameraTarget = GetAllHorseMiddlePosition() + (toLastVec / moveSecond) * (horseCloseUpTimer - moveSecond);
	}
	
	_cameraPosition = _cameraTarget+SECOND_CORNER_MIDDLE_CAMERA_TARGET_OFFSET * horseManager->GetHorse(0)->GetRotationMatrix();

	horseCloseUpTimer += Time::DeltaTime();
	if (horseManager->GetHorse(0)->GetRemainingDistance() < SECOND_CORNER_MIDDLE_CAMERA_TRANSITION_REMAINING_DISTANCE)
	{
		nextSegment = FINAL_CORNER;
	}
}

void WatchCamera::FinalCornerUpdate(VECTOR3& _cameraPosition, VECTOR3& _cameraTarget)
{
	if (prevSegment != currentSegment)
		cameraChangeRequest = true;

	_cameraPosition = FINAL_CORNER_CAMERA_POSITION;
	_cameraTarget = horseManager->GetHorse(0)->GetPosition();

	_cameraPosition = CommonCameraFunc::ZoomIn(_cameraPosition, _cameraTarget, FINAL_CORNER_CAMERA_ZOOMIN_DISTANCE);

	if (course->GetCurrentSegmentType(_cameraTarget) == CourseDataManager::HOME_STREATCH)
	{
		nextSegment = HOME_STREATCH;
	}
}

void WatchCamera::HomeStreatchUpdate(VECTOR3& _cameraPosition, VECTOR3& _cameraTarget)
{
	if (prevSegment != currentSegment)
		cameraChangeRequest = true;

	VECTOR3 goalPos = course->GetGoalLineData().innerPos - VECTOR3(course->GetCourseWidth(), 0, 0);
	//1位馬の座標を取る
	_cameraTarget = horseManager->GetHorse(0)->GetPosition();
	_cameraPosition = goalPos + VECTOR3(0, HOME_STREATCH_CAMERA_TARGET_OFFSET.y, 0);
	_cameraPosition.z = _cameraTarget.z + HOME_STREATCH_CAMERA_TARGET_OFFSET.z;

	float targetDistance = VSize(_cameraPosition - _cameraTarget);

	//遷移条件　ToDo上に移動させる
	if (_cameraPosition.z > goalPos.z)
	{
		nextSegment = GOAL;
		_cameraPosition.z = goalPos.z;
		return;
	}
	_cameraPosition = CommonCameraFunc::ZoomIn(_cameraPosition, _cameraTarget, HOME_STREATCH_CAMERA_ZOOMIN_DISTANCE);
}

void WatchCamera::GoalUpdate(VECTOR3& _cameraPosition, VECTOR3& _cameraTarget)
{
	_cameraTarget = horseManager->GetHorse(0)->GetPosition();
	VECTOR3 goalPos = course->GetGoalLineData().innerPos - VECTOR3(course->GetCourseWidth(), 0, 0);
	_cameraTarget.z = min(goalPos.z, _cameraTarget.z);

	float targetDistance = VSize(_cameraPosition - _cameraTarget);

	if (horseManager->GetHorse(0)->GetIsGoal())
	{
		nextSegment = AFTER_GOAL;
	}

	//ズーム
	/*if (targetDistance > 2000.0f)
	{
		VECTOR3 positionToTargetVec = VNorm(_cameraTarget - _cameraPosition);
		_cameraPosition += positionToTargetVec * (targetDistance - 2000.0f);
	}*/
}

VECTOR3 WatchCamera::GetAllHorseMiddlePosition()
{
	VECTOR3 allPos;
	for (const auto& h : horseManager->GetHorses())
	{
		allPos += h->GetPosition();
	}

	return allPos / (float)horseManager->GetHorses().size();
}
