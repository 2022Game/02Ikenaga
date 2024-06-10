#pragma once
#include "CObjectBase.h"
#include "CWeapon.h"
#include "CModel.h"
#include "CColliderSphere.h"

// 波動エフェクト
class CWave : public CWeapon
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">エフェクトを発生させたオブジェクト</param>
	/// <param name="pos">発生位置</param>
	/// <param name="dir">移動方向</param>
	/// <param name="speed">移動速度</param>
	/// <param name="dist">移動したら消える距離</param>
	CWave(CObjectBase* owner, const CVector& pos, const CVector& dir, float speed, float dist);
	
	// デストラクタ
	~CWave();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 攻撃開始
	void AttackStart() override;
	// 攻撃終了
	void AttackEnd() override;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CObjectBase* mpOwner;	// このエフェクトの持ち主
	CModel* mpModel;		// エフェクトのモデルデータ
	CVector mMoveSpeed;		// 移動速度
	float mKillMoveDist;	// 移動したら消える距離
	float mMovedDist;		// 現在移動した距離
	CColliderSphere* mpAttackCol;
};