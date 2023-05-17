#pragma once
#include <Windows.h>
#include "GLFW/glfw3.h"
#include <map>

class CInput
{
public:
	static void Window(GLFWwindow* pwindow);
	void GetMousePos(float* px, float* py);

	CInput();
	//bool Key(文字)
	//戻り値
	//true:文字のキーが押されている
	//false:文字のキーが押されていない
	bool Key(int key);

	//キーを押した瞬間かどうか
	//前回のフレームは離した状態で
	//今回のフレームは押している状態の時だけtrueを返す
	bool PushKey(int key);

	//キーを離した瞬間かどうか
	//前回のフレームは押している状態で
	//今回のフレームは離した状態の時だけtrueを返す
	bool PullKey(int key);

	//キーの入力状態を更新
	//(呼び出しは1フレームに1回まで)
	static void Update();
private:
	static GLFWwindow* spWindow;	//ウィンドウのポインタ

	//キーの入力状態ビット
	enum InputBit
	{
		eInputBit_On,	//押しているかどうか
		eInputBit_Push,	//押した瞬間かどうか
		eInputBit_Pull,	//離した瞬間かどうか
		Num,
	};
	//キーの入力状態を管理するリスト
	static std::map<int, int> msInputBits;
};