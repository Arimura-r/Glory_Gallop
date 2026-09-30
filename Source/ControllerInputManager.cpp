#include "controllerInputManager.h"
#include "inputManager.h"
//#include "../ImGui/imgui.h"

ControllerInputManager::ControllerInputManager()
{
	playerNumber = PLAYER_1;
	DontDestroyOnSceneChange(true);
}

ControllerInputManager::~ControllerInputManager()
{
	input.clear();
}

void ControllerInputManager::Update()
{
	GetJoypadXInputState(playerNumber, &inputNow);
	switch (playerNumber)
	{
	case PLAYER_1:
		GetJoypadXInputState(PLAYER_1, &inputNow);
		break;
	case PLAYER_2:
		GetJoypadXInputState(PLAYER_2, &inputNow);
		break;
	case PLAYER_3:
		GetJoypadXInputState(PLAYER_3, &inputNow);
		break;
	case PLAYER_4:
		GetJoypadXInputState(PLAYER_4, &inputNow);
		break;
	}

	//現フレームの入力をリストの先頭に保存する
	input.push_front(inputNow);
	//保存する最大フレーム数以上になったら一番昔の入力を削除
	if (input.size() > InputManager::SAVE_FRAME_MAX)
	{
		input.pop_back();
	}

	/*ImGui::Begin("PadInput");
	ImGui::Text("%d %d %d %d", GetPadInputPut(P_L_TRIGGER), GetPadInputPut(P_R_TRIGGER), GetPadInputPut(P_L_STICK_DOWN), GetPadInputPut(P_R_STICK_DOWN_RIGHT));
	ImGui::End();*/
}

bool ControllerInputManager::GetAnyButtonPush(bool _getStick, int _backFrame)
{
	if (input.size() < 1)
	{
		return false;
	}

	std::list<XINPUT_STATE>::iterator it = input.begin();

	//それ以上戻れない場合や0フレーム(現在のフレームを取ろうとしている)場合は飛ばす
	if ((int)input.size() > _backFrame + 1 && _backFrame != 0)
	{
		std::advance(it, _backFrame);
	}

	//いずれかのボタンが押されたらtrueを返す
	for (int i = 0; i < BUTTON_MAX; i++)
	{
		if ((*it).Buttons[i])
		{
			return true;
		}

	}

	//トリガーがデッドゾーンより押されたらtrueを返す
	if ((*it).LeftTrigger > ANY_KEY_TRIGGER_DEADZONE * 255 || (*it).RightTrigger > ANY_KEY_TRIGGER_DEADZONE * 255)
	{
		return true;
	}

	//スティックも取る設定ならスティックも取る
	if (_getStick)
	{
		//GetStickKnockingの返り値がS_NO_DIRECTIONではない=いずれかの方向に入力がされている
		if (GetStickKnocking(ANY_KEY_STICK_DEADZONE, _backFrame).leftStick != S_NO_DIRECTION || GetStickKnocking(ANY_KEY_STICK_DEADZONE, _backFrame).rightStick != S_NO_DIRECTION)
		{
			return true;
		}
	}

	//上のどれにも当てはまらないならfalseを返す
	return false;
}

bool ControllerInputManager::GetAnyButtonPush(bool _getStick)
{
	return GetAnyButtonPush(_getStick, 0);
}

bool ControllerInputManager::GetAnyButtonPut(bool _getStick)
{
	return (GetAnyButtonPush(_getStick, 0) && !GetAnyButtonPush(_getStick, 1));
}

bool ControllerInputManager::GetIsButtonPushingNow(int _button)
{
	if (input.front().Buttons[_button])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ControllerInputManager::GetIsButtonPutNow(int _button)
{
	if (input.size() > 1)
	{
		std::list<XINPUT_STATE>::iterator it = input.begin();
		XINPUT_STATE now = *it;
		std::advance(it, 1);
		XINPUT_STATE lastFrame = *it;

		if (now.Buttons[_button] && !lastFrame.Buttons[_button])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool ControllerInputManager::GetIsButtonReleaseNow(int _button)
{
	if (input.size() > 1)
	{
		std::list<XINPUT_STATE>::iterator it = input.begin();
		XINPUT_STATE now = *it;
		std::advance(it, 1);
		XINPUT_STATE lastFrame = *it;

		if (!now.Buttons[_button] && lastFrame.Buttons[_button])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool ControllerInputManager::GetPadInputPushing(PadInputType _padInput)
{
	if (_padInput==P_NONE)
	{
		return false;
	}
	bool ret = false;
	//P_BUTTON_Y以下の場合は通常のX_INPUT_BUTTONで押されているかを取得する
	if ((int)_padInput <= P_BUTTON_Y)
	{
		ret = GetIsButtonPushingNow(_padInput);
	}
	else if (_padInput == P_L_TRIGGER || _padInput == P_R_TRIGGER)
	{
		//トリガーの場合はGetTrigger関数を使って押されているかを取得する
		StickFloat trigger = GetTriggerNow();
		if (_padInput == P_L_TRIGGER)
		{
			ret = (trigger.leftStick > TRIGGER_DEADZONE);
		}
		else
		{
			ret = (trigger.rightStick > TRIGGER_DEADZONE);
		}
	}
	else//残り(スティック)の場合はGetStickKnockingを使って調べる
	{
		StickDirection sDir = GetStickKnocking(DEFAULT_STICK_DEADZONE);

		//左右のスティックで分ける
		if (_padInput <= P_L_STICK_DOWN_LEFT)
		{
			const int enumDifL = P_L_STICK_DOWN - S_DOWN;
			ret = (sDir.leftStick == (StickDirections)(_padInput - enumDifL));
		}
		else
		{
			const int enumDifR = P_R_STICK_DOWN - S_DOWN;
			ret = (sDir.rightStick == (StickDirections)(_padInput - enumDifR));
		}
	}
	return ret;
}

bool ControllerInputManager::GetPadInputPut(PadInputType _padInput)
{
	if (_padInput == P_NONE)
	{
		return false;
	}
	bool ret = false;
	//P_BUTTON_Y以下の場合は通常のX_INPUT_BUTTONで押されているかを取得する
	if ((int)_padInput <= P_BUTTON_Y)
	{
		ret = GetIsButtonPutNow(_padInput);
	}
	else if (_padInput == P_L_TRIGGER || _padInput == P_R_TRIGGER)
	{
		//トリガーの場合はGetTrigger関数を使って押されているかを取得する
		StickFloat trigger = GetTriggerPut(TRIGGER_DEADZONE);
		if (_padInput == P_L_TRIGGER)
		{
			ret = (trigger.leftStick > TRIGGER_DEADZONE);
		}
		else
		{
			ret = (trigger.rightStick > TRIGGER_DEADZONE);
		}
	}
	else//残り(スティック)の場合はGetStickKnockingを使って調べる
	{
		StickDirection sDir = GetStickKnockingPut(DEFAULT_STICK_DEADZONE);

		//左右のスティックで分ける
		if (_padInput <= P_L_STICK_DOWN_LEFT)
		{
			const int enumDifL = P_L_STICK_DOWN - S_DOWN;
			ret = (sDir.leftStick == (StickDirections)(_padInput - enumDifL));
		}
		else
		{
			const int enumDifR = P_R_STICK_DOWN - S_DOWN;
			ret = (sDir.rightStick == (StickDirections)(_padInput - enumDifR));
		}
	}
	return ret;
}

/*bool ControllerInputManager::GetIsButtonPushingPast(int _button, int _frameNum)
{
	std::list<XINPUT_STATE>::iterator it = input.begin();
	XINPUT_STATE now = *it;

	//最大値より大きい値が入ったらそのままfalseを返す
	if (_frameNum >= input.size())
	{
		return false;
	}
	std::advance(it, _frameNum);

	if (it->Buttons[_button])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ControllerInputManager::GetIsButtonPutPast(int _button, int _frameNum)
{
	std::list<XINPUT_STATE>::iterator it = input.begin();

	//最大値より大きい値が入ったらそのままfalseを返す
	if (_frameNum >= input.size() - 1)
	{
		return false;
	}
	std::advance(it, _frameNum);

	XINPUT_STATE now = *it;
	std::advance(it, 1);
	XINPUT_STATE lastFrame = *it;

	if (now.Buttons[_button] && !lastFrame.Buttons[_button])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ControllerInputManager::GetIsButtonReleasePast(int _button, int _frameNum)
{
	std::list<XINPUT_STATE>::iterator it = input.begin();

	//最大値より大きい値が入ったらそのままfalseを返す
	if (_frameNum >= input.size() - 1)
	{
		return false;
	}
	std::advance(it, _frameNum);

	XINPUT_STATE now = *it;
	std::advance(it, 1);
	XINPUT_STATE lastFrame = *it;

	if (!now.Buttons[_button] && lastFrame.Buttons[_button])
	{
		return true;
	}
	else
	{
		return false;
	}
}*/

int ControllerInputManager::GetPushButtonTime(int _button)
{
	if (input.size() < _button)
	{
		return -1;
	}
	std::list<XINPUT_STATE>::iterator it = input.begin();
	XINPUT_STATE now = *it;

	if (!now.Buttons[_button])
	{
		return 0;
	}

	int time = 0;
	do
	{
		time++;
		if (time >= input.size())
		{
			break;
		}
		std::advance(it, 1);
		now = *it;
	} while (now.Buttons[_button]);

	return time;
}

StickInput ControllerInputManager::GetStickInput()
{
	StickInput result;
	result.leftStick = { (float)inputNow.ThumbLX / STICK_INPUT_MAX,(float)inputNow.ThumbLY / STICK_INPUT_MAX };
	result.rightStick = { (float)inputNow.ThumbRX / STICK_INPUT_MAX,(float)inputNow.ThumbRY / STICK_INPUT_MAX };

	return result;
}

StickInput ControllerInputManager::GetStickInput(int _backFrame)
{
	StickInput result;
	auto it = input.begin();
	//std::next(it, _frame);
	if (_backFrame > 0 && input.size() > _backFrame)
	{
		std::advance(it, _backFrame);//入力されたフレーム分イテレータを進める
	}

	result.leftStick = { (float)(*it).ThumbLX / STICK_INPUT_MAX,(float)(*it).ThumbLY / STICK_INPUT_MAX };
	result.rightStick = { (float)(*it).ThumbRX / STICK_INPUT_MAX,(float)(*it).ThumbRY / STICK_INPUT_MAX };

	return result;
}

StickDirection ControllerInputManager::GetStickKnocking(float _deadZone)
{
	StickDirection result{ S_NO_DIRECTION };
	StickFloat stickRot = GetStickRot();
	//ベクトルの量を求める
	StickFloat stickSize{ 0 };
	stickSize.leftStick = sqrtf(GetStickInput().leftStick.x * GetStickInput().leftStick.x + GetStickInput().leftStick.y * GetStickInput().leftStick.y);
	stickSize.rightStick = sqrtf(GetStickInput().rightStick.x * GetStickInput().rightStick.x + GetStickInput().rightStick.y * GetStickInput().rightStick.y);

	if (stickSize.leftStick > _deadZone)
	{
		result.leftStick = GetDirection(stickRot.leftStick);
	}
	if (stickSize.rightStick > _deadZone)
	{
		result.rightStick = GetDirection(stickRot.rightStick);
	}

	return result;
}

StickDirection ControllerInputManager::GetStickKnocking(float _deadZone, int _backFrame)
{
	StickDirection result{ S_NO_DIRECTION };
	StickFloat stickRot = GetStickRot(_backFrame);
	//ベクトルの量を求める
	StickFloat stickSize{ 0 };
	stickSize.leftStick = sqrtf(GetStickInput(_backFrame).leftStick.x * GetStickInput(_backFrame).leftStick.x + GetStickInput(_backFrame).leftStick.y * GetStickInput(_backFrame).leftStick.y);
	stickSize.rightStick = sqrtf(GetStickInput(_backFrame).rightStick.x * GetStickInput(_backFrame).rightStick.x + GetStickInput(_backFrame).rightStick.y * GetStickInput(_backFrame).rightStick.y);

	if (stickSize.leftStick > _deadZone)
	{
		result.leftStick = GetDirection(stickRot.leftStick);
	}
	if (stickSize.rightStick > _deadZone)
	{
		result.rightStick = GetDirection(stickRot.rightStick);
	}

	return result;
}

StickDirection ControllerInputManager::GetStickKnockingPut(float _deadZone)
{
	StickDirection result{ S_NO_DIRECTION,S_NO_DIRECTION };

	//現フレームの入力
	StickDirection nowInput = GetStickKnocking(_deadZone);
	//過去フレームの入力
	StickDirection lastInput = GetStickKnocking(_deadZone, 1);

	if (nowInput.leftStick != lastInput.leftStick)
	{
		result.leftStick = nowInput.leftStick;
	}
	if (nowInput.rightStick != lastInput.rightStick)
	{
		result.rightStick = nowInput.rightStick;
	}

	return result;
}

StickFloat ControllerInputManager::GetStickRot()
{
	StickFloat result{ 0 };
	StickInput stick = GetStickInput();

	result.leftStick = atan2f((float)inputNow.ThumbLY, (float)inputNow.ThumbLX);
	result.rightStick = atan2f((float)inputNow.ThumbRY, (float)inputNow.ThumbRX);

	return result;
}

StickFloat ControllerInputManager::GetStickRot(int _backFrame)
{
	StickFloat result{ 0 };
	StickInput stick = GetStickInput(_backFrame);

	result.leftStick = atan2f((float)inputNow.ThumbLY, (float)inputNow.ThumbLX);
	result.rightStick = atan2f((float)inputNow.ThumbRY, (float)inputNow.ThumbRX);

	return result;
}

StickFloat ControllerInputManager::GetTriggerNow()
{
	StickFloat trigger{ inputNow.LeftTrigger / 255.0f, inputNow.RightTrigger / 255.0f };
	return trigger;
}

StickFloat ControllerInputManager::GetTriggerPut(float _deadzone)
{
	if (input.size() <= 1)
	{
		return StickFloat{ 0,0 };
	}
	//前フレームのトリガーと現フレームのトリガーの入手
	auto it = input.begin();
	it++;
	StickFloat lastTrigger = { (*it).LeftTrigger / 255.0f,(*it).RightTrigger / 255.0f };
	StickFloat currentTrigger = GetTriggerNow();

	//DeadZoneの指定
	if (currentTrigger.rightStick < _deadzone)
		currentTrigger.rightStick = 0;
	if (currentTrigger.leftStick < _deadzone)
		currentTrigger.leftStick = 0;
	if (lastTrigger.rightStick < _deadzone)
		lastTrigger.rightStick = 0;
	if (lastTrigger.leftStick < _deadzone)
		lastTrigger.leftStick = 0;

	StickFloat ret = currentTrigger;

	//前フレームの入力があったなら入力を0にする
	if (lastTrigger.leftStick > 0)
		ret.leftStick = 0;
	if (lastTrigger.rightStick > 0)
		ret.rightStick = 0;
	
	return ret;
}

void ControllerInputManager::ControlVibrationStartTime(int _power, int _timeSecond)
{
	StartJoypadVibration(playerNumber, _power, _timeSecond * 1000);
}

void ControllerInputManager::ControlVibrationStartFrame(int _power, int _timeFrame)
{
	StartJoypadVibration(playerNumber, _power, _timeFrame * 1000 / 60);
}

StickDirections ControllerInputManager::GetDirection(float _rot)
{
	_rot += DX_PI_F;//計算しやすい用に整数にする

	const float directionAngleMax = DX_PI_F / 8;

	if (_rot < directionAngleMax * 1)
	{
		return S_LEFT;
	}
	else if (_rot < directionAngleMax * 3)
	{
		return S_DOWN_LEFT;
	}
	else if (_rot < directionAngleMax * 5)
	{
		return S_DOWN;
	}
	else if (_rot < directionAngleMax * 7)
	{
		return S_DOWN_RIGHT;
	}
	else if (_rot < directionAngleMax * 9)
	{
		return S_RIGHT;
	}
	else if (_rot < directionAngleMax * 11)
	{
		return S_UP_RIGHT;
	}
	else if (_rot < directionAngleMax * 13)
	{
		const float a = DX_PI_F + DX_PI_F / 6;
		return S_UP;
	}
	else if (_rot < directionAngleMax * 15)
	{
		return S_UP_LEFT;
	}

	return S_LEFT;

}