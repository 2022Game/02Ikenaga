#pragma once
#include "CTask.h"
#include "CImage.h"

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
	CImage* mpTextOperation;
	CImage* mpTextMove;
	CImage* mpTextSpecial;
	CImage* mpTextSP;
	CImage* mpTextMenu;
	CImage* mpTextJump;
	CImage* mpTextAvoid;
	CImage* mpTextMove2;
	CImage* mpTextAttack;
	CImage* mpTextGuard;
	// キー関連
	CImage* mpWkey;
	CImage* mpAkey;
	CImage* mpSkey;
	CImage* mpDkey;
	CImage* mpQkey;
	CImage* mpEkey;
	CImage* mpMkey;
	CImage* mpSpaceKey;
	CImage* mpShiftKey;
	// マウス関連
	CImage* mpMouseLeft;
	CImage* mpMouseMiddle;
	CImage* mpMouseRight;
	// 記号関連
	CImage* mpPlus;

	bool mIsOpened;
};