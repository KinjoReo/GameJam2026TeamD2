//#pragma once
//
//#include "InputState.h"
//
//struct Stick
//{
//	float ThumbX;
//	float ThumbY;
//};
//
//class InputManager
//{
//private:
//	static InputManager* instance;
//	//ボタン&キーの取得
//private:
//	char now_key[256];
//	char old_key[256];
//	char now_button[2][16];
//	char old_button[2][16];
//	float left_trigger[2];
//	float right_trigger[2];
//	Stick left_stick[2];
//	Stick right_stick[2];
//	int playerid;
//
//private:
//	InputManager();
//	~InputManager() = default;
//	
//public:
//	//擬似的なコンストラクタ
//	static InputManager* GetInstance();
//	//疑似的なデストラクト
//	static void DeleteInstance();
//
//public:
//	void Update();
//
//	eInputState GetKeyState(int key) const;
//	eInputState GetButtonState(int button, int id) const;
//	float GetLeftTrigger(int id) const;
//	float GetRightTrigger(int id) const;
//	Stick GetLeftStick(int id);
//	Stick GetRightStick(int id);
//
//private:
//	bool CheckKeycodeRange(int key)const;
//	bool CheckButtonRange(int button)const;
//};