#include "CDebugTimer.h"

// コンストラクタ
CDebugTimer::CDebugTimer()
	: freq({ 0 })
	, startCount({ 0 })
	, endCount({ 0 })
	, isEnd(false)
{
}

// 時間計測開始
void CDebugTimer::Start()
{
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&startCount);
	isEnd = false;
}

// 時間計測終了
void CDebugTimer::End()
{
	QueryPerformanceCounter(&endCount);
	isEnd = true;
}

// 計測した時間を取得
float CDebugTimer::Get() const
{
	if (isEnd)
	{
		return (float)(endCount.QuadPart - startCount.QuadPart) / freq.QuadPart;
	}
	else
	{
		LARGE_INTEGER tempCount;
		QueryPerformanceCounter(&tempCount);
		return (float)(tempCount.QuadPart - startCount.QuadPart) / freq.QuadPart;
	}
}

// 時間計測を終了しているか
bool CDebugTimer::IsEnd() const
{
	return isEnd;
}
