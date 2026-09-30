#include "Camera.h"
#include "CourseDataManager.h"
#include "HorseManager.h"
#include "HorsePlayer.h"
#include "../ImGui/imgui.h"
#include "CommonManager.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "WatchCamera.h"
#include "CommonCameraFunc.h"
#include "GameManager.h"

namespace
{
	//定数　ToDo将来的には外部化orクラス分けする
	const float ALL_OVERLOOK_CAMERA_HEIGHT = 50000.0f;//全体俯瞰カメラの高さ
	const float FIRST_OVERLOOK_CAMERA_HEIGHT = 5000.0f;//先頭追従俯瞰カメラの高さ
	const float PLAYER_OVERLOOK_CAMERA_HEIGHT_DEFAULT = 3000.0f;//プレイヤー追従俯瞰カメラの高さ
	const float FREE_OVERLOOK_CAMERA_HEIGHT_DEFAULT = 5000.0f;//フリー見下ろしカメラの通常の高さ

	VECTOR3 PLAYER_FOLLOW_CAMERA_TO_PLAYER_VEC = VECTOR3(0, 350, -500);//プレイヤー追従カメラのプレイヤーからの相対座標
	VECTOR3 PLAYER_FOLLOW_CAMERA_TARGET_VEC = VECTOR3(0, -100, 1000);//プレイヤー追従カメラの目標地点へのベクトル

	const float OVERLOOK_MAX_CAMERA_HEIGHT = 10000.0f;

	const float DEFAULT_CAMERA_NEAR = 300.0f;
	const float DEFAULT_CAMERA_FAR = 170000.0f;

	const float FREE_CAMERA_MOVE_SPEED = 50.0f;
	const float FREE_CAMERA_ZOOM_SPEED = 100.0f;
}

const float Camera::DEFAULT_CAMERA_MOVE_SPEED_MAX = 3000.0f;

Camera::Camera(HorseManager* _horse, CourseDataManager* _course, GameManager* _gameManager, CommonManager* _common, bool _canPlayerControll)
{
	prevState = ALL_OVERLOOK;
	currentState = ALL_OVERLOOK;
	nextState = ALL_OVERLOOK;

	cameraTargetHorse = _horse->GetPlayer();;
	horseManager = _horse;
	course = _course;
	gameManager = _gameManager;
	player = horseManager->GetPlayer();
	input = _common->FindCommonObject<InputManager>();
	sound = _common->FindCommonObject<SoundManager>();
	overlookCameraHeight = PLAYER_OVERLOOK_CAMERA_HEIGHT_DEFAULT;
	freeOverlookCameraPosition.y = FREE_OVERLOOK_CAMERA_HEIGHT_DEFAULT;
	freeOverlookCameraTarget.x -= 0.01f;//全くpositionと同じ座標だと映らないため、少しだけ動かす
	followHorseIndex = 0;
	followRacePosition = 0;
	cameraMoveSpeed = DEFAULT_CAMERA_MOVE_SPEED_MAX;
	SetCameraNearFar(DEFAULT_CAMERA_NEAR, DEFAULT_CAMERA_FAR);

	watchCamera = new WatchCamera(_course,_horse);
}

Camera::~Camera()
{
	if (sound->CheckPlay("Cheer"))
		sound->Stop("Cheer");
}

void Camera::Update()
{
	switch (currentState)
	{
	case Camera::ALL_OVERLOOK:
		AllOverlookCameraUpdate();
		break;
	case Camera::FIRST_OVERLOOK:
		//ToDo 実装を後回しにしている(Playerの流用にする)
		FirstOverlookCameraUpdate();
		break;
	case Camera::PLAYER_FOLLOW:
		PlayerFollowCameraUpdate();
		break;
	case Camera::PLAYER_OVERLOOK:
		PlayerOverlookCameraUpdate();
		break;
	case Camera::HORSE_FOLLOW:
		HorseFollowCameraUpdate();
		break;
	case Camera::HORSE_OVERLOOK:
		HorseOverlookCameraUpdate();
		break;
	case Camera::RACE_POSITION_OVERLOOK:
		RacePositionOverlookCameraUpdate();
		break;
	case Camera::WATCH_CAMERA:
		watchCamera->CameraUpdate(cameraPosition,cameraTarget);
		cameraMoveSpeed=watchCamera->GetCameraMoveSpeedRequest();
		if (prevState != currentState)
			CameraTypeChange();
		break;
	case Camera::FREE_OVERLOOK:
		FreeOverlookCameraUpdate();
		break;
	default:
		break;
	}

	//前とカメラの方式が違うなら、瞬間的にカメラを移動させる
	/*if (lastState != state)
	{
		SetCameraPositionAndTarget_UpVecY(cameraPosition, cameraTarget);
		lastState = state;
		CameraTypeChange();
	}
	else */if (watchCamera->GetCameraChangeRequest())
	{
		CameraTypeChange();
	}
	else
	{
		//補間をかける
		CameraPositionApply();
	}

#if _DEBUG
	ImGuiUpdate();
#endif

	//カメラの注目対象が最終直線にいるなら歓声を入れる
	if (course->GetCurrentSegmentType(cameraTarget)==CourseDataManager::HOME_STREATCH)
	{
		sound->PlayOnce("Cheer",50,true);
	}

	if (nextState != currentState)
		currentState = nextState;
	else
		prevState = currentState;
}

void Camera::ImGuiUpdate()
{
	ImGui::Begin("Camera");
	if (ImGui::ListBoxHeader("CurrentCameraType"))
	{
		if (ImGui::Selectable("ALL_OVERLOOK", currentState==ALL_OVERLOOK))
		{
			nextState = ALL_OVERLOOK;
		}
		/*if (ImGui::Selectable("FIRST_OVERLOOK", currentState == FIRST_OVERLOOK))
		{
			currentState = FIRST_OVERLOOK;
		}*/
		if (ImGui::Selectable("PLAYER_FOLLOW", currentState == PLAYER_FOLLOW))
		{
			if(player!=nullptr)
			{
				nextState = PLAYER_FOLLOW;
				cameraTargetHorse=player;
			}
		}
		if (ImGui::Selectable("PLAYER_OVERLOOK", currentState == PLAYER_OVERLOOK))
		{
			if (player != nullptr)
			{
				nextState = PLAYER_OVERLOOK;
				cameraTargetHorse = player;
			}
		}
		if (ImGui::Selectable("HORSE_FOLLOW", currentState == HORSE_FOLLOW))
		{
			nextState = HORSE_FOLLOW;
		}
		if (ImGui::Selectable("HORSE_OVERLOOK", currentState == HORSE_OVERLOOK))
		{
			currentState = HORSE_OVERLOOK;
		}
		if (ImGui::Selectable("RACE_POSITION_OVERLOOK", currentState == RACE_POSITION_OVERLOOK))
		{
			nextState = RACE_POSITION_OVERLOOK;
		}
		if (ImGui::Selectable("WATCH_CAMERA", currentState == WATCH_CAMERA))
		{
			nextState = WATCH_CAMERA;
		}
		if (ImGui::Selectable("FREE_OVERLOOK", currentState == FREE_OVERLOOK))
		{
			nextState = FREE_OVERLOOK;
		}
		ImGui::ListBoxFooter();
	}
	switch (currentState)
	{
	case Camera::ALL_OVERLOOK:
		break;
	case Camera::FIRST_OVERLOOK:
		break;
	case Camera::PLAYER_FOLLOW:
		break;
	case Camera::PLAYER_OVERLOOK:
		ImGui::SliderFloat("CameraHeight", &overlookCameraHeight, 0, 10000.0f);//カメラの高さを調整
		break;
	case Camera::HORSE_FOLLOW:
		ImGui::SliderInt("FollowHorse", &followHorseIndex, 0, (int)horseManager->GetHorses().size() - 1);
		break;
	case Camera::HORSE_OVERLOOK:
		ImGui::SliderInt("FollowHorse", &followHorseIndex, 0, (int)horseManager->GetHorses().size()-1);
		break;
	case Camera::RACE_POSITION_OVERLOOK:
		ImGui::SliderInt("FollowRacePosition", &followRacePosition, 0, (int)horseManager->GetHorses().size() - 1);
		ImGui::SliderFloat("CameraHeight", &overlookCameraHeight, 0, 10000.0f);//カメラの高さを調整
		break;
	case Camera::STATE_NUM:
		break;
	default:
		break;
	}

	ImGui::InputFloat3("Position", &cameraPosition.x);
	ImGui::InputFloat3("Target", &cameraTarget.x);
	ImGui::End();
}

void Camera::AllOverlookCameraUpdate()
{
	//X,Zをコースの中間地点に置く
	float cameraX = course->GetLeftCornerCenter().x;
	float cameraZ = (course->GetGoalLineData().innerPos.z + course->GetLeftCornerCenter().z) / 2.0f;

	lastCameraPosition = VECTOR3(cameraX + 1, ALL_OVERLOOK_CAMERA_HEIGHT, cameraZ);
	lastCameraTarget = VECTOR3(cameraX, 0, cameraZ);
	cameraPosition = lastCameraPosition;
	cameraTarget = lastCameraTarget;

	if (gameManager->GetChangeAllViewToPlayCamera())
	{
		if (player == nullptr)
			nextState = WATCH_CAMERA;
		else
			nextState = PLAYER_FOLLOW;
	}

	if (prevState != currentState)
		CameraTypeChange();
}

void Camera::FirstOverlookCameraUpdate()
{
}

//ToDo　HorseFollow,Overlookと処理が被っているので共通化する
void Camera::PlayerFollowCameraUpdate()
{
	VECTOR3 pPos = player->GetPosition();
	cameraPosition = pPos + PLAYER_FOLLOW_CAMERA_TO_PLAYER_VEC * player->GetRotationMatrix();
	cameraTarget = cameraPosition + PLAYER_FOLLOW_CAMERA_TARGET_VEC * player->GetRotationMatrix();

	if (prevState != currentState)
		CameraTypeChange();
}

void Camera::PlayerOverlookCameraUpdate()
{
	VECTOR3 pPos = player->GetPosition();
	//Xが全く同じだと描画されないため少し動かす
	//SetCameraPositionAndTarget_UpVecY(VECTOR3(pPos.x + 1, playerOverlookCameraHeight, pPos.z), VECTOR3(pPos.x, 0, pPos.z));
	cameraPosition = VECTOR3(pPos.x + 1, overlookCameraHeight, pPos.z);
	cameraTarget = VECTOR3(pPos.x, 0, pPos.z);
	//カメラとプレイヤーの距離に応じてNearFarを調整する
	//SetCameraNearFar(playerOverlookCameraHeight/10.0f, playerOverlookCameraHeight*10.0f);

	if (prevState != currentState)
		CameraTypeChange();
}

void Camera::HorseFollowCameraUpdate()
{
	Horse* h = horseManager->GetHorses()[followHorseIndex];

	cameraPosition = h->GetPosition() + PLAYER_FOLLOW_CAMERA_TO_PLAYER_VEC * h->GetRotationMatrix();
	cameraTarget = cameraPosition + PLAYER_FOLLOW_CAMERA_TARGET_VEC * h->GetRotationMatrix();

	cameraTargetHorse = h;

	if (prevState != currentState)
		CameraTypeChange();
}

void Camera::HorseOverlookCameraUpdate()
{
	Horse* h = horseManager->GetHorses()[followHorseIndex];
	const VECTOR3& hPos = h->GetPosition();

	cameraPosition = VECTOR3(hPos.x + 1, overlookCameraHeight, hPos.z);
	cameraTarget = VECTOR3(hPos.x, 0, hPos.z);

	cameraTargetHorse = h;

	if (prevState != currentState)
		CameraTypeChange();
}

void Camera::RacePositionOverlookCameraUpdate()
{
	Horse* followHorse = nullptr;
	for (const auto& h : horseManager->GetHorses())
	{
		if (h->GetRacePosition() == followRacePosition)
			followHorse = h;
	}

	if (followHorse==nullptr)
		return;

	const VECTOR3& hPos = followHorse->GetPosition();

	cameraPosition = VECTOR3(hPos.x + 1, overlookCameraHeight, hPos.z);
	cameraTarget = VECTOR3(hPos.x, 0, hPos.z);

	cameraTargetHorse = followHorse;
}

void Camera::FreeOverlookCameraUpdate()
{
	if (input->GetKeyboardInput()->GetIsKeyboardPushing(KEY_INPUT_D))
	{
		freeOverlookCameraPosition.z += FREE_CAMERA_MOVE_SPEED;
		freeOverlookCameraTarget.z += FREE_CAMERA_MOVE_SPEED;
	}
	if (input->GetKeyboardInput()->GetIsKeyboardPushing(KEY_INPUT_A))
	{
		freeOverlookCameraPosition.z -= FREE_CAMERA_MOVE_SPEED;
		freeOverlookCameraTarget.z -= FREE_CAMERA_MOVE_SPEED;
	}
	if (input->GetKeyboardInput()->GetIsKeyboardPushing(KEY_INPUT_W))
	{
		freeOverlookCameraPosition.x -= FREE_CAMERA_MOVE_SPEED;
		freeOverlookCameraTarget.x -= FREE_CAMERA_MOVE_SPEED;
	}
	if (input->GetKeyboardInput()->GetIsKeyboardPushing(KEY_INPUT_S))
	{
		freeOverlookCameraPosition.x += FREE_CAMERA_MOVE_SPEED;
		freeOverlookCameraTarget.x += FREE_CAMERA_MOVE_SPEED;
	}
	if (input->GetKeyboardInput()->GetIsKeyboardPushing(KEY_INPUT_Z))
	{
		freeOverlookCameraPosition.y -= FREE_CAMERA_ZOOM_SPEED;
	}
	if (input->GetKeyboardInput()->GetIsKeyboardPushing(KEY_INPUT_X))
	{
		freeOverlookCameraPosition.y += FREE_CAMERA_ZOOM_SPEED;
	}
	
	cameraPosition = freeOverlookCameraPosition;
	cameraTarget = freeOverlookCameraTarget;
	CameraTypeChange();
	//SetCameraPositionAndTarget_UpVecY(freeOverlookCameraPosition, freeOverlookCameraTarget);
}

void Camera::CameraPositionApply()
{
	/*VECTOR3 position = cameraPosition;
	VECTOR3 target = cameraTarget;
	if (VSquareSize(cameraPosition - lastCameraPosition) > cameraMoveSpeed * cameraMoveSpeed * Time::DeltaTime() * Time::DeltaTime())
	{
		VECTOR3 positionVec = VNorm(cameraPosition - lastCameraPosition);
		position = lastCameraPosition + positionVec * cameraMoveSpeed * Time::DeltaTime();
	}

	if (VSquareSize(cameraTarget - lastCameraTarget) > cameraMoveSpeed * cameraMoveSpeed * Time::DeltaTime() * Time::DeltaTime())
	{
		VECTOR3 targetVec = VNorm(cameraTarget - lastCameraTarget);
		target=lastCameraTarget + targetVec * cameraMoveSpeed * Time::DeltaTime();
	}*/

	VECTOR3 position = CommonCameraFunc::MoveBitweenPosition(lastCameraPosition, cameraPosition, cameraMoveSpeed);
	VECTOR3 target = CommonCameraFunc::MoveBitweenPosition(lastCameraTarget, cameraTarget, cameraMoveSpeed);

	SetCameraPositionAndTarget_UpVecY(position, target);

	lastCameraPosition = position;
	lastCameraTarget = target;
}

void Camera::CameraTypeChange()
{
	lastCameraPosition = cameraPosition;
	lastCameraTarget = cameraTarget;
}
