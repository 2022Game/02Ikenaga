#pragma once
#include <Windows.h>
#include "GLFW/glfw3.h"
#include <map>
#include "CVector.h"

// 入力判定クラス
class CInput
{
public:
	/// <summary>
	/// ウィンドウを設定
	/// </summary>
	/// <param name="pwindow"></param>
	static void Window(GLFWwindow* pwindow);
	// マウス座標を取得
	static CVector2 GetMousePos();

	/// <summary>
	/// キーが入力されているか
	/// </summary>
	/// <param name="key">判定するキー</param>
	/// <returns>trueならば、押されている</returns>
	static bool Key(int key);

	/// <summary>
	/// キーを押した瞬間かどうか
	/// 前回のフレームは押していない状態で
	/// 今回のフレームで押した状態になった時の
	/// 1フレームのみtrueを返す
	/// </summary>
	/// <param name="key">判定するキー</param>
	/// <returns>trueならば、押した瞬間である</returns>
	static bool PushKey(int key);

	/// <summary>
	/// キーを離した瞬間かどうか
	/// 前回のフレームは押している状態で
	/// 今回のフレームで離した状態になった時の
	/// 1フレームのみtrueを返す
	/// </summary>
	/// <param name="key">判定するキー</param>
	/// <returns>trueならば、離した瞬間である</returns>
	static bool PullKey(int key);

	/// <summary>
	/// キーの入力状態を更新
	/// （呼び出しは1フレームに1回まで）
	/// </summary>
	static void Update();

private:
	// コンストラクタ
	// （インスタンスを生成できないようにprivate）
	CInput();

	static GLFWwindow* spWindow;	// ウィンドウのポインタ

	// キーの入力状態ビット
	enum InputBit
	{
		eInputBit_On,	//押しているかどうか
		eInputBit_Push,	//押した瞬間かどうか
		eInputBit_Pull,	//離した瞬間かどうか
		Num,
	};
	// キーの入力状態を管理するリスト
	static std::map<int, int> msInputBits;
};