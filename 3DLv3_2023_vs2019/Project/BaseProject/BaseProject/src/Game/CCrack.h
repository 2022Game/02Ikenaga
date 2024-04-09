#pragma once
#include "CBillBoardImage.h"

// ひび割れ
class CCrack : public CBillBoardImage
{
public:
	// コンストラクタ
	CCrack(ETag tag);
	// デストラクタ
	~CCrack();

	/// <summary>
	/// 各パラメータを設定
	/// </summary>
	/// <param name="pos">初期位置</param>
	/// <param name="dir">移動方向</param>
	/// <param name="speed">移動速度</param>
	void Setup(const CVector& pos, const CVector& dir, float speed);

	// カラーを設定
	void SetColor(const CColor& color) override;
	// ブレンドタイプを設定
	void SetBlendType(EBlend type);

	// 削除フラグが立っているかどうか
	bool IsDeath() const;

	// 更新
	void Update() override;

private:
	CVector mMoveSpeed;	// 移動速度
	float mElapsedTime;	// 経過時間
	bool mIsDeath;		// 削除フラグ
};