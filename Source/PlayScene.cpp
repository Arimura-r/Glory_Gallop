#include "PlayScene.h"
#include "CommonManager.h"
#include "HorseManager.h"
#include "GoalChecker.h"
#include "CourseDataManager.h"
#include "Camera.h"
#include "UIManager.h"
#include "GameManager.h"
#include "Stage.h"
#include "Sky.h"
#include "SoundManager.h"
#include "GameStartData.h"
#include "TicketManager.h"
#include "../ImGui/imgui.h"

PlayScene::PlayScene()
{
	//hModel = MV1LoadModel("data\\model\\character\\horse_01.mv1");
	auto common = FindGameObject<CommonManager>();
	SetLightDirection(VECTOR3(0.5f, -1, 0.5f));
	CreateDirLightHandle(VECTOR3(-0.5f,-0.3f, -0.5f));

	sound=common->FindCommonObject<SoundManager>();
	sound->LoadResource("PLAY");

	auto startData = common->FindCommonObject<GameStartData>()->GetData();

	auto stage=new Stage();
	auto course=new CourseDataManager();
	auto horse = new HorseManager(course, common, stage, startData.canPlayerControll);
	new GoalChecker(horse,course);
	auto ticket = new TicketManager(horse,common);
	auto gameManager = new GameManager(horse,common,ticket);
	auto camera=new Camera(horse, course, gameManager, common, startData.canPlayerControll);
	auto UI = new UIManager(gameManager, course, horse, ticket, camera, startData.canPlayerControll);
	new Sky();

	gameSpeedRate = 1.0f;
}

PlayScene::~PlayScene()
{
	sound->DeleteResource("PLAY");
	DeleteLightHandleAll();
}

void PlayScene::Update()
{
#if _DEBUG
	ImGui::Begin("GameSpeed");
	ImGui::SliderFloat("GameSpeed", &gameSpeedRate, 0, 20.0f);
	if (ImGui::Button("Apply"))
		Time::ChangeSpeedRate(gameSpeedRate);
	if (ImGui::Button("Reset"))
		Time::ChangeSpeedRate(1.0f);
	if (ImGui::Button("Stop"))
		Time::ChangeSpeedRate(0.0f);
	ImGui::End();
#endif
}

void PlayScene::Draw()
{
	//DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	//DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
	//DrawSphere3D(VECTOR3(0, 0, 0), 10, 10, 0xff0000, 0xff0000, true);

	//MV1DrawModel(hModel);
}
