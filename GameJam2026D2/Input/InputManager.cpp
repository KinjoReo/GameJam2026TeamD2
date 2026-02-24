#include "InputManager.h"
#include "DxLib.h"

InputManager* InputManager::instance = nullptr;

InputManager::InputManager()
	:now_key{}
	, old_key{}
	, now_button{}
	, old_button{}
	, left_trigger{}
	, right_trigger{}
	, left_stick{}
	, right_stick{}
	, playerid{}
{

}

InputManager* InputManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new InputManager();
	}
	return instance;
}

void InputManager::DeleteInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

void InputManager::Update()
{
	memcpy(old_key, now_key, sizeof(char) * 256);
	GetHitKeyStateAll(now_key);

	for (int i = 0; i < 2; i++)
	{
		memcpy(old_button[i], now_button[i], sizeof(char) * 16);
		XINPUT_STATE input = {};
		if (i == 0)
		{
			playerid = DX_INPUT_PAD1;
		}

		if (GetJoypadXInputState(playerid, &input) == ERROR_SUCCESS)
		{

		}
		memcpy(now_button[i], input.Buttons, sizeof(char) * 16);

		left_trigger[i] = input.LeftTrigger / 255.f;
		right_trigger[i] = input.RightTrigger / 255.f;

		left_stick[i].ThumbX = input.ThumbLX / 32767.f;
		left_stick[i].ThumbY = input.ThumbLY / 32767.f;
		right_stick[i].ThumbX = input.ThumbRX / 32767.f;
		right_stick[i].ThumbY = input.ThumbRY / 32767.f;
	}

}

eInputState InputManager::GetKeyState(int key)
{
	if (CheckKeycodeRange(key))
	{
		if (old_key[key] == TRUE)
		{
			if (now_key[key] == TRUE)
			{
				return eInputState::eHold;
			}
			else
			{
				return eInputState::eReleased;
			}
		}
		else
		{
			if (now_key[key] == TRUE)
			{
				return eInputState::ePressed;
			}
		}
	}
	return eInputState::eNone;
}

bool InputManager::CheckKeycodeRange(int key)const
{
	return(0 <= key && key < 256);
}

eInputState InputManager::GetButtonState(int button, int id) const
{
	if (CheckButtonRange(button) == TRUE)
	{
		if (old_button[id][button] == TRUE)
		{
			if (now_button[id][button] == TRUE)
			{
				return eInputState::eHold;
			}
			else
			{
				return eInputState::eReleased;
			}
		}
		else
		{
			if (now_button[id][button] == TRUE)
			{
				return eInputState::ePressed;
			}
		}
	}
	return eInputState::eNone;
}

bool InputManager::CheckButtonRange(int button)const
{
	return(0 <= button && button < 16);
}

float InputManager::GetLeftTrigger(int id) const
{
	return left_trigger[id];
}

float InputManager::GetRightTrigger(int id) const
{
	return right_trigger[id];
}

Stick InputManager::GetLeftStick(int id)
{
	return left_stick[id];
}

Stick InputManager::GetRightStick(int id)
{
	return right_stick[id];
}
