#include "CInput.h"
#include <stdio.h>

GLFWwindow* CInput::spWindow = nullptr;	// ウィンドウのポインタ
std::map<int, int> CInput::msInputBits;	// キーの入力状態を管理するリスト

// ウィンドウを設定
void CInput::Window(GLFWwindow* pwindow)
{
	spWindow = pwindow;
}

// マウス座標を取得
CVector2 CInput::GetMousePos()
{
	double xpos, ypos;
	glfwGetCursorPos(spWindow, &xpos, &ypos);
	return CVector2((float)xpos, (float)ypos);
}

// コンストラクタ
CInput::CInput()
{
	printf("入力インスタンスが生まれました\n");
}

// キーが入力されているか
bool CInput::Key(int key)
{
	return (msInputBits[key] & (1 << eInputBit_On)) != 0;
}

// キーを押した瞬間かどうか
bool CInput::PushKey(int key)
{
	return (msInputBits[key] & (1 << eInputBit_Push)) != 0;
}

// キーを離した瞬間かどうか
bool CInput::PullKey(int key)
{
	return (msInputBits[key] & (1 << eInputBit_Pull)) != 0;
}

// キーの入力状態を更新
void CInput::Update()
{
	auto itBegin = msInputBits.begin();
	auto itEnd = msInputBits.end();
	for (auto it = itBegin; it != itEnd; ++it)
	{
		int key = it->first;
		int bit = it->second;

		// 現在押している状態か取得
		bool isOn = (GetAsyncKeyState(key) & 0x8000) != 0;
		// 前回押している状態か取得
		bool isLastOn = bit & (1 << eInputBit_On);

		// ビットフラグ初期化
		bit = 0;

		// 現在押している状態であれば、
		// eInputBit_Onのビットを立てる
		if (isOn) bit |= (1 << eInputBit_On);
		// 前回は離していて、現在は押している状態であれば、
		// eInputBit_Pushのビットを立てる
		if (!isLastOn && isOn) bit |= (1 << eInputBit_Push);
		// 前回は押していて、現在は離している状態であれば、
		// eInputBit_Pullのビットを立てる
		if (isLastOn && !isOn) bit |= (1 << eInputBit_Pull);

		// リスト内のビットフラグを上書き
		it->second = bit;
	}
}
