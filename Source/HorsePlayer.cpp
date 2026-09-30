#include "HorsePlayer.h"
#include "CommonManager.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "Animator.h"

HorsePlayer::HorsePlayer(VECTOR3 _pos, int _horseNumber, int _horseNum, int _model, COLOR_F  _color, const HorseParam& _param, CourseDataManager* _course, Animator* _anim, CommonManager* _common):Horse(_pos, _horseNumber,_horseNum,_model,_color,_param,_course,_anim,_common)
{
	input = common->FindCommonObject<InputManager>();
}

HorsePlayer::~HorsePlayer()
{
}

void HorsePlayer::Update()
{
	Horse::Update();

	//レース中以外は操作が出来ないようにする
	if (not outGate)
		return;

	//外に移動する
	if (CheckHitKey(KEY_INPUT_A))
	{
		MoveOut();
	}
	//内に移動する
	else if (CheckHitKey(KEY_INPUT_D))
	{
		MoveIn();
	}
	if (input->GetControllerInput()->GetPadInputPut(P_BUTTON_B)|| input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_W))
	{
		//スタート時なら
		if (param.speed.currentSpeed < param.speed.baseSpeed &&
			param.speed.currentTargetSpeed >= param.speed.baseSpeed)
		{
			StartSpeedUp();
		}
		else
		{
			SpeedUp();
		}
	}
	else if (input->GetControllerInput()->GetPadInputPushing(P_BUTTON_A)||CheckHitKey(KEY_INPUT_S))
	{
		if(param.speed.currentSpeed>param.speed.minSpeed)
			SpeedDown();
	}
	if (input->GetControllerInput()->GetStickInput().leftStick.x > STICK_DEADZONE)
	{
		//右に移動する処理
		MoveIn();
	}
	else if (input->GetControllerInput()->GetStickInput().leftStick.x < -STICK_DEADZONE)
	{
		//左に移動する処理
		MoveOut();
	}
	if (input->GetControllerInput()->GetPadInputPushing(P_L_TRIGGER) || input->GetControllerInput()->GetPadInputPushing(P_R_TRIGGER)||CheckHitKey(KEY_INPUT_Z))
	{
		//スタミナがない時のみ鞭を入れられるようにする
		if(param.stamina.GetCurrentStaminaRate()<0.1f)
		{
			bool ret = Whip();
			if (ret)
				sound->Play("Whip");
		}
	}

	if (anim->GetCurrentAnimCurrentFrame() > 16.0f)
	{
		StartJoypadVibration(DX_INPUT_PAD1, 300+(int)max((param.speed.currentSpeed-param.speed.baseSpeed)*5,0), 2000);
	}
	else
	{
		StopJoypadVibration(DX_INPUT_PAD1);
	}

	if (param.speed.currentSpeed == param.speed.currentTargetSpeed&&
		param.speed.currentTargetSpeed>param.speed.baseSpeed)
		AutoDecel();
}

void HorsePlayer::GoalIn()
{
	Horse::GoalIn();
	sound->Play("Goal");
}
