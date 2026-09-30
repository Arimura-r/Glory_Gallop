#include "keyboardInputManager.h"

//const int KeyboardInputManager::KEYBOARD_BUF_NUM = 256;

KeyboardInputManager::KeyboardInputManager()
{
	DontDestroyOnSceneChange(true);
}

KeyboardInputManager::~KeyboardInputManager()
{
	keyboardInput.clear();
}

void KeyboardInputManager::Update()
{
	GetHitKeyStateAll(keyboardInputNow.kInput);

	keyboardInput.push_front(keyboardInputNow);
	if (keyboardInput.size() > InputManager::SAVE_FRAME_MAX)
	{
		keyboardInput.pop_back();
	}
}


bool KeyboardInputManager::GetIsKeyboardPushing(int _keyCode)
{
	if (keyboardInput.front().kInput[_keyCode])
	{
		return true;
	}
	return false;
}

bool KeyboardInputManager::GetIsKeyboardPut(int _keyCode)
{
	if(keyboardInput.size()>1)
	{
		auto it = keyboardInput.begin();
		KeyboardInput now = *it;
		std::advance(it, 1);
		KeyboardInput lastFrame = *it;

		if (now.kInput[_keyCode] && !lastFrame.kInput[_keyCode])
		{
			return true;
		}
	}
	return false;
}

bool KeyboardInputManager::GetIsKeyboardRelease(int _keyCode)
{
	if (keyboardInput.size() > 1)
	{
		std::list<KeyboardInput>::iterator it = keyboardInput.begin();
		KeyboardInput now = *it;
		std::advance(it, 1);
		KeyboardInput lastFrame = *it;

		if (!now.kInput[_keyCode] && lastFrame.kInput[_keyCode])
		{
			return true;
		}
		else
		{
			return false;
		}

	}
	return false;
}