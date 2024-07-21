#pragma once
#include "CTask.h"
#include "CImage.h"
#include <vector>

class COperationIns : public CTask
{
public:
	// コンストラク
	COperationIns();
	// デストラクタ
	~COperationIns();

	// オープン
	void Open();
	// クローズ
	void Close();
	// オープンするかどうか
	bool IsOpened() const;

	// 決める
	void Decide(int select);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CImage* mpBackground;
	// テキスト関連
	CImage* mpText;
	CImage* mpText2;
	CImage* mpText3;
	CImage* mpTextSP;
	CImage* mpText4;
	CImage* mpText5;
	CImage* mpText6;
	CImage* mpText7;
	// キー関連
	CImage* mpWkey;
	CImage* mpAkey;
	CImage* mpSkey;
	CImage* mpDkey;
	CImage* mpQkey;
	CImage* mpEkey;
	CImage* mpMkey;
	CImage* mpSpaceKey;
	// マウス関連
	CImage* mpMouseLeft;
	CImage* mpMouseMiddle;
	CImage* mpMouseRight;

	bool mIsOpened;
};