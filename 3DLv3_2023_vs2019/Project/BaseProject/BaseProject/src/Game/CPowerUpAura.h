#pragma once
#include "CBillBoardImage.h"

// パワーアップオーラ
class CPowerUpAura : public CBillBoardImage
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="angle">角度</param>
	/// <param name="width">幅</param>
	/// <param name="dire">移動方向</param>
	/// <param name="speed">移動速度</param>
	/// <param name="dist">移動したら消える距離</param>
	CPowerUpAura(float angle, float width, const CVector& dire, float speed, float dist);

	// デストラクタ
	~CPowerUpAura();

	// 持ち主を設定
	void SetOwner(CCharaBase* owner) override;

	// カラーを設定
	void SetColor(const CColor& color) override;

	// ブレンドタイプを設定
	void SetBlendType(EBlend type);

	// パワーアップオーラ開始
	void StartAura();
	// パワーアップオーラ終了
	void EndAura();

	// 更新
	void Update();

private:

	// 攻撃力アップ
	bool mPowerUp;
	// 経過時間(攻撃力アップ用)
	float mElapsedPowerUpTime;

	float mAngle;        // 回転角度
	float mDistance;     // 持ち主からの距離
	CVector mMoveDir;	 // 移動する方向ベクトル
	CVector mMoveSpeed;	 // 移動速度
	float mKillMoveDist; // 移動したら消える距離
	float mMovedDist;    // 現在移動した距離
	float mBaseScale;    // 持ち主のベーススケール値
};