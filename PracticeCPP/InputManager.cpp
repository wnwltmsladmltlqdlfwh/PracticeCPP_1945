#include "pch.h"
#include "InputManager.h"

void InputManager::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_states.resize(KEY_TYPE_COUNT, KeyState::None);
}

void InputManager::Update()
{
	// ::GetAsyncKeyState() << 계속 호출하게 되면 속도가 느려짐

	BYTE asciiKeys[KEY_TYPE_COUNT] = {};

	if (::GetKeyboardState(asciiKeys) == false) // 매 프레임마다 입력장치의 상태를 확인 (GetAsyncKeyState보다 빠름)
		return;

	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// 키가 눌려 있으면 true
		if (asciiKeys[key] & 0x80)
		{
			KeyState& state = _states[key];

			// 이전 프레임에 키를 누른 상태라면 PRESS
			if (state == KeyState::Press || state == KeyState::Down)
				state = KeyState::Press;
			else
				state = KeyState::Down;
		}
		else
		{
			KeyState& state = _states[key];

			// 이전 프레임에 키를 누른 상태라면 UP
			if (state == KeyState::Press || state == KeyState::Down)
				state = KeyState::Up;
			else
				state = KeyState::None;
		}
	}

	// Mouse
	::GetCursorPos(&_mousePos);	// 커서의 좌표를 알아온다.
	::ScreenToClient(_hwnd, &_mousePos);
}
