#pragma once
#include "CSceneBase.h"
#include "CSceneManager.h"
#include "CImage.h"
#include "CInput.h"

//タイトルシーン
class CTitleScene :public CSceneBase
{
public:
	//コンストラクタ
	CTitleScene();
	//デストラクタ
	~CTitleScene();

	//シーン読み込み
	void Load();
	void Update();
private:
	CInput mInput;
	CImage *mBgImage;
};