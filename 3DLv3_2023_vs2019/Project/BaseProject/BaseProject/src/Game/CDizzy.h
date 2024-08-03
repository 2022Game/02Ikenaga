#pragma once
#include "CBillBoardImage.h"

// めまい
class CDizzy : public CBillBoardImage
{
public:
	// コンストラクタ
	CDizzy();
	// デストラクタ
	~CDizzy();

	// 位置を設定
	void Setup(const CVector& pos);

	// カラーを設定
	void SetColor(const CColor& color) override;
	// ブレンドタイプを設定
	void SetBlendType(EBlend type);

	// 削除フラグが立っているかどうか
	bool IsDeath() const;

	// 更新
	void Update() override;

private:
	// アニメーションデータ
	static TexAnimData msAnimData;
	float mElapsedTime;	// 経過時間
	bool mIsDeath;		// 削除フラグ
};