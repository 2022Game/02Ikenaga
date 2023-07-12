#pragma once
#include "CTask.h"
#include "CInput.h"
#include "CImage.h"

//ゲームポーズ制御クラス
class GamePause : public CTask
{
private:
	CInput mInput;			//入力判定用
	bool mIsPaused;			//ポーズ中かどうか
	int mPauseStep;			//ポーズ処理のステップ
	float mElapsedTime;		//経過時間計測用
	float mPauseTextAlpha;	//PAUSEテキストのアルファ値保存用

	CImage* mFadeBg;		//フェード背景のイメージ
	CImage* mPauseText;		//PAUSEテキストのイメージ

public:
	//コンストラクタ
	GamePause();
	//デストラクタ
	~GamePause();

	//更新
	void Update();
	//描画
	void Render();
};