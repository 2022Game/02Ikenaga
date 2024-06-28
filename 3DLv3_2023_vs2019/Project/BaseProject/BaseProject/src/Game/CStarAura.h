#pragma once
#include "CBillBoardImage.h"

// 星オーラ
class CStarAura : public CBillBoardImage
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">発生位置</param>
	/// <param name="dir">移動方向</param>
	/// <param name="speed">移動速度</param>
	/// <param name="dist">移動したら消える距離</param>
	CStarAura(const CVector& pos, const CVector& dir, float speed, float dist);
	// デストラクタ
	~CStarAura();

	// カラーを設定
	void SetColor(const CColor& color) override;
	// ブレンドタイプを設定
	void SetBlendType(EBlend type);

	// 更新
	void Update() override;

private:
	CVector mMoveDir;	  // 移動する方向ベクトル
	float mElapsedTime;	  // 経過時間
	CVector mMoveSpeed;	  // 移動速度
	float mKillMoveDist;  // 移動したら消える距離
	float mMovedDist;     // 現在移動した距離
};