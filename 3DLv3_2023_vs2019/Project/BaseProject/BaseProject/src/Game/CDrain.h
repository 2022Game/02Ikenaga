#pragma once
#include "CBillBoardImage.h"
#include "CColliderSphere.h"

// ドレイン(HP吸収)
class CDrain : public CBillBoardImage
{
public:
	// コンストラクタ
	CDrain(ETag tag);

	// デストラクタ
	~CDrain();

	/// <summary>
	/// 各パラメータを設定
	/// </summary>
	/// <param name="pos">初期位置</param>
	/// <param name="dir">移動方向</param>
	/// <param name="speed">移動速度</param>
	void Setup(const CVector& pos, const CVector& dir, float dist);

	// カラーを設定
	void SetColor(const CColor& color) override;

	// ブレンドタイプを設定
	void SetBlendType(EBlend type);

	// 削除フラグが立っているかどうか
	bool IsDeath() const;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 更新
	void Update() override;

private:
	CVector mMoveDir;	     // 移動する方向ベクトル
	bool mIsDeath;		     // 削除フラグ
	float mKillMoveDist;     // 移動したら消える距離
	float mMovedDist;        // 現在移動した距離
	bool mIsDrain;           // ドレインするかどうか
	float mElapsedTime;	     // 経過時間
	float mElapsedDrainTime; // ドレインの経過時間
	CColliderSphere* mpAttackCollider;
};