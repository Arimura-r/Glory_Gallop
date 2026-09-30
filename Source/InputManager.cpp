#include "inputManager.h"
#include "../ImGui/imgui.h"

namespace
{
	//キーコンフィグ　ここの値を使って操作入力を行うと後からデータの変更が容易
	//std::vector<KeyConfigData> DEFAULT_KEY_CONFIGS;
		/*{
		KeyConfigData("TEST",P_L_STICK_DOWN_LEFT,KEY_INPUT_W,M_CURSOR_UP),
		KeyConfigData("ChangeTitleScene",P_NONE,KEY_INPUT_T,M_NO_CONTROL),
		KeyConfigData("ChangePlayScene",P_NONE,KEY_INPUT_P,M_NO_CONTROL),
	};*/
}
const int InputManager::SAVE_FRAME_MAX = 60;

InputManager::InputManager(bool _useController, bool _useKeyboard, bool _useMouse)
{
	controller = nullptr;
	keyboard = nullptr;
	mouse = nullptr;
	if (_useController)
	{
		controller = new ControllerInputManager();
	}
	if (_useKeyboard)
	{
		keyboard = new KeyboardInputManager();
	}
	if (_useMouse)
	{
		mouse = new MouseInputManager();
	}

	DontDestroyOnSceneChange(true);

	/*for (int i = 0; i < DEFAULT_KEY_CONFIGS.size(); i++)
	{
		keyConfig.emplace(DEFAULT_KEY_CONFIGS[i].keyName, DEFAULT_KEY_CONFIGS[i].inputData);
	}*/
}

InputManager::~InputManager()
{
}

void InputManager::Update()
{
}

bool InputManager::GetKeyConfigPush(std::string _keyName) const
{
	//std::unordered_map<std::string, InputConfigData>::iterator it = keyConfig.find(_keyName);
	std::_List_const_iterator it= keyConfig.find(_keyName);

	//要素が見つからなかったらfalseを返す
	if (it == keyConfig.end())
	{
		return false;
	}
	
	//コントローラー、キーボード、マウスで値が入っているものに対して入力チェックを行うが、いずれかがTrueならTrueで返す
	InputConfigData inputData = (*it).second;
	bool ret = false;
	if (controller != nullptr && !ret && inputData.padInput >= 0)
	{
		ret = controller->GetPadInputPushing(inputData.padInput);
	}
	if (keyboard != nullptr && !ret && inputData.keyboardInput >= 0)
	{
		ret = keyboard->GetIsKeyboardPushing(inputData.keyboardInput);
	}
	if (mouse != nullptr && !ret&&inputData.mouseInput>=0)
	{
		ret = mouse->GetMouseControlPushing(inputData.mouseInput);
	}

	return ret;
}

bool InputManager::GetKeyConfigPut(std::string _keyName) const
{
	//std::unordered_map<std::string, InputConfigData>::iterator it = keyConfig.find(_keyName);
	std::_List_const_iterator it = keyConfig.find(_keyName);

	//要素が見つからなかったらfalseを返す
	if (it == keyConfig.end())
	{
		return false;
	}

	//コントローラー、キーボード、マウスで値が入っているものに対して入力チェックを行うが、いずれかがTrueならTrueで返す
	InputConfigData inputData = (*it).second;
	bool ret = false;
	if (controller != nullptr && !ret && inputData.padInput >= 0)
	{
		ret = controller->GetPadInputPut(inputData.padInput);
	}
	if (keyboard != nullptr && !ret && inputData.keyboardInput >= 0)
	{
		ret = keyboard->GetIsKeyboardPut(inputData.keyboardInput);
	}
	if (mouse != nullptr && !ret && inputData.mouseInput >= 0)
	{
		ret = mouse->GetMouseControlPut(inputData.mouseInput);
	}

	return ret;
}

int InputManager::CreateOrChangekeyConfig(KeyConfigData _configData)
{
	auto it = keyConfig.find(_configData.keyName);
	
	//キーが存在しないなら新しく作る
	if (it == keyConfig.end())
	{
		keyConfig.emplace(_configData.keyName,_configData.inputData);
		return 1;
	}
	else
	{//存在しているならその要素を書き換える
		(*it).second = _configData.inputData;
		return 0;
	}

	return -1;
}

ControllerInputManager* InputManager::GetControllerInput()
{
	if (controller != nullptr)
	{
		return controller;
	}
	return nullptr;
}

KeyboardInputManager* InputManager::GetKeyboardInput()
{
	if (keyboard != nullptr)
	{
		return keyboard;
	}
	return nullptr;
}

MouseInputManager* InputManager::GetMouseInput()
{
	if(mouse!=nullptr)
	{
		return mouse;
	}
	return nullptr;
}

/*bool InputManager::GetIsKeyOrButtonPushingNow(int _key, int _button)
{
	if(GetIsKeyPushingNow(_button)||GetIsKeyboardPushing(_key))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputManager::GetIsKeyOrButtonPutNow(int _key, int _button)
{
	if (GetIsKeyPutNow(_button) || GetIsKeyboardPut(_key))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputManager::GetIsKeyOrButtonReleaseNow(int _key, int _button)
{
	if (GetIsKeyReleaseNow(_button) || GetIsKeyboardRelease(_key))
	{
		return true;
	}
	else
	{
		return false;
	}
}*/