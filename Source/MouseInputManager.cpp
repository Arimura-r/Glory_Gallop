#include "mouseInputManager.h"
#include "Screen.h"
#include "inputManager.h"
//#include "../ImGui/imgui.h"

MouseInputManager::MouseInputManager()
{
	SetMouseDispFlag(true);
	DontDestroyOnSceneChange(true);
	SetUseDirectInputFlag(true);
	nowMouseWheelVol = 0;
	SetDrawMouseCursor(true);
}

MouseInputManager::~MouseInputManager()
{
	buttonInput.clear();
}

void MouseInputManager::Update()
{
	nowMouseWheelVol = GetMouseWheelRotVol();

	//カーソルポジションの更新
	VECTOR2 newPos = GetMousePos(true);
	cursorPosDif = newPos - currentCursorPos;
	currentCursorPos = newPos;

	/*ImGui::Begin("Mouse");
	ImGui::Text("Current%f %f Dif%f %f", currentCursorPos.x, currentCursorPos.y, cursorPosDif.x, cursorPosDif.y);
	ImGui::Text("RIGHT%d LEFT%d UP%d DOWN%d", GetMousePushing(RIGHT_CLICK), IsCursorMove(CURSOR_LEFT), IsCursorMove(CURSOR_UP), IsCursorMove(CURSOR_DOWN));
	ImGui::Text("RIGHT%d LEFT%d WHEEL%d", GetMouseControlPushing(M_CURSOR_LEFT), GetMouseControlPushing(M_CURSOR_DOWN), GetMouseControlPushing(M_CURSOR_UP));
	ImGui::End();*/

	PushInputList();
}

VECTOR2 MouseInputManager::GetMousePos(bool _canScreenOut)
{
	int x, y;
	GetMousePoint(&x,&y);
	VECTOR2 result((float)x, (float)y);

	//falseなら画面外の値を画面端にする
	if(!_canScreenOut)
	{
		if (result.x > Screen::WIDTH)
		{
			result.x = Screen::WIDTH;
		}
		else if (result.x < 0)
		{
			result.x = 0;
		}
		if (result.y > Screen::HEIGHT)
		{
			result.y = Screen::HEIGHT;
		}
		else if (result.y < 0)
		{
			result.y = 0;
		}
	}
	return result;
}

void MouseInputManager::SetMousePos(VECTOR2 _pos)
{
	if (!canSetCursorPos)
		return;

	SetMousePoint((int)_pos.x, (int)_pos.y);
}

bool MouseInputManager::GetMousePushing(MouseButton _button)
{
	return currentButtonInput.input[_button];
}

bool MouseInputManager::GetMouseControlPushing(MouseInputType _controlType)
{
	if (_controlType == M_NO_CONTROL)
	{
		return false;
	}

	bool ret = false;

	//クリック系の入力対応
	if (_controlType == M_RIGHT_CLICK || _controlType == M_LEFT_CLICK || _controlType == M_WHEEL_BUTTON)
	{
		ret = GetMousePushing((MouseButton)_controlType);
	}
	//ホイールの入力対応
	else if (_controlType == M_WHEEL_UP || _controlType == M_WHEEL_DOWN)
	{
		int wheelNum = GetMouseWheelFrame();
		if ((wheelNum > 0 && _controlType == M_WHEEL_UP) || (wheelNum < 0 && _controlType == M_WHEEL_DOWN))
		{
			ret = true;
		}
	}
	else if (_controlType == M_CURSOR_RIGHT || _controlType == M_CURSOR_LEFT || _controlType == M_CURSOR_UP || _controlType == M_CURSOR_DOWN)
	{
		//CURSORとM_CURSOR_RIGHTの差で引数を合わせる
		const int enumDif = M_CURSOR_RIGHT - CURSOR_RIGHT;
		ret = IsCursorMove((CursorMoveDirectionType)((int)_controlType - enumDif));
	}
	return ret;
}

bool MouseInputManager::GetMouseControlPut(MouseInputType _controlType)
{
	if (_controlType == M_NO_CONTROL)
	{
		return false;
	}

	bool ret = false;

	//クリック系の入力対応
	if (_controlType == M_RIGHT_CLICK || _controlType == M_LEFT_CLICK || _controlType == M_WHEEL_BUTTON)
	{
		ret = GetMousePut((MouseButton)_controlType);
	}
	//ホイールの入力対応
	else if (_controlType == M_WHEEL_UP || _controlType == M_WHEEL_DOWN)
	{
		int wheelNum = GetMouseWheelFrame();
		if ((wheelNum > 0 && _controlType == M_WHEEL_UP) || (wheelNum < 0 && _controlType == M_WHEEL_DOWN))
		{
			ret = true;
		}
	}
	else if (_controlType == M_CURSOR_RIGHT || _controlType == M_CURSOR_LEFT || _controlType == M_CURSOR_UP || _controlType == M_CURSOR_DOWN)
	{
		//CURSORとM_CURSOR_RIGHTの差で引数を合わせる
		const int enumDif = M_CURSOR_RIGHT - CURSOR_RIGHT;
		ret = IsCursorMove((CursorMoveDirectionType)((int)_controlType - enumDif));
	}
	return ret;
}

bool MouseInputManager::GetMousePut(MouseButton _button)
{
	if(buttonInput.size()>1)
	{
		auto it = buttonInput.begin();
		MouseButtonInput currentInput = *it;
		it++;
		MouseButtonInput lastFrameInput = *it;

		return (currentInput.input[_button]) && (!lastFrameInput.input[_button]);
	}
	return false;
}

int MouseInputManager::GetMouseWheelFrame()const
{
	return nowMouseWheelVol;
}

bool MouseInputManager::IsCursorSquareCollision(VECTOR2 _centerPos, VECTOR2 _size)
{
	VECTOR2 leftUpPos = _centerPos - _size;
	VECTOR2 rightDownPos = _centerPos + _size;
	VECTOR2 mousePos = GetMousePos(false);

	if (leftUpPos.x < mousePos.x && mousePos.x < rightDownPos.x &&
		leftUpPos.y < mousePos.y && mousePos.y < rightDownPos.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool MouseInputManager::IsCursorMove(CursorMoveDirectionType _cursorDirection, bool _setCursorCenter)
{
	bool ret = false;
	switch (_cursorDirection)
	{
	case MouseInputManager::NO_DIRECTION:
		break;
	case MouseInputManager::CURSOR_RIGHT:
		if (cursorPosDif.x >= CHECK_CURSOR_MOVE_VEC_SIZE)
		{
			ret = true;
		}
		break;
	case MouseInputManager::CURSOR_LEFT:
		if (cursorPosDif.x <= -CHECK_CURSOR_MOVE_VEC_SIZE)
		{
			ret = true;
		}
		break;
	case MouseInputManager::CURSOR_UP:
		if (cursorPosDif.y <= -CHECK_CURSOR_MOVE_VEC_SIZE)
		{
			ret = true;
		}
		break;
	case MouseInputManager::CURSOR_DOWN:
		if (cursorPosDif.y >= CHECK_CURSOR_MOVE_VEC_SIZE)
		{
			ret = true;
		}
		break;
	default:
		break;
	}

	if (ret&&_setCursorCenter)
	{
		SetMousePos(VECTOR2(Screen::WIDTH_CENTER, Screen::HEIGHT_CENTER));
		currentCursorPos = GetMousePos(true);//中心にカーソルを戻したときは前フレームとのカーソル座標の差を取らないようにする
	}

	return ret;
}

void MouseInputManager::SetCurrentButtonInput()
{
	int input= GetMouseInput();

	currentButtonInput.input[LEFT_CLICK] = (input & MOUSE_INPUT_LEFT);
	currentButtonInput.input[RIGHT_CLICK] = (input & MOUSE_INPUT_RIGHT);
	currentButtonInput.input[WHEEL_BUTTON] = (input & MOUSE_INPUT_MIDDLE);
	currentButtonInput.input[BUTTON_4] = (input & MOUSE_INPUT_4);
	currentButtonInput.input[BUTTON_5] = (input & MOUSE_INPUT_5);
	currentButtonInput.input[BUTTON_6] = (input & MOUSE_INPUT_6);
	currentButtonInput.input[BUTTON_7] = (input & MOUSE_INPUT_7);
	currentButtonInput.input[BUTTON_8] = (input & MOUSE_INPUT_8);
}

void MouseInputManager::PushInputList()
{
	SetCurrentButtonInput();
	buttonInput.push_front(currentButtonInput);
	if (InputManager::SAVE_FRAME_MAX < buttonInput.size())
	{
		buttonInput.pop_back();
	}
}
