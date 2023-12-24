#pragma once
class CGameScene;

class CApplication
{
private:
	enum class EState
	{
		EPLAY,	//ゲーム中
		ECLEAR,	//ゲームクリア
		EOVER,	//ゲームオーバー
	};
	CGameScene* mpGameScene;
	EState mState;
public:
	~CApplication();

	//最初に一度だけ実行するプログラム
	void Start();
	//終了時に実行するプログラム
	void End();
	//繰り返し実行するプログラム
	void Update();
};