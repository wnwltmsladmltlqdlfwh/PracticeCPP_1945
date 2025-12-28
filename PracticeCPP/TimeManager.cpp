#include "pch.h"
#include "TimeManager.h"

void TimeManager::Init()
{
	// GetTickCount 시리즈 >> 특정 시간을 기준으로 경과된 시간을 ms 단위로 리턴해줌
	//	정밀도가 조금 떨어짐

	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(& _frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount));
}

void TimeManager::Update()
{
	uint64 currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);
	_prevCount = currentCount;

	_frameCount++;
	_frameTime += _deltaTime;

	if (_frameTime >= 1.f)
	{
		_fps = static_cast<uint32>(_frameCount / _frameTime);

		_frameTime = 0.f;
		_frameCount = 0;
	}
}
