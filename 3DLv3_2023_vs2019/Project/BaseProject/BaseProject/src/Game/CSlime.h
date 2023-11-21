#pragma once
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderSphere.h"
#include "CRideableObject.h"
class CHpGauge;

#include <algorithm>
/*
レッドスライム(エネミー)のクラス
キャラクタクラスを継承
*/
class CSlime : public CXCharacter
{
public:
	bool IsDeath() const;
	//インスタンスのポインタの取得
	static CSlime* Instance();

	// コンストラクタ
	CSlime();
	~CSlime();

	// 待機状態
	void UpdateIdle();
	// 待機2状態
	void UpdateIdle2();
	// プレイヤーの攻撃がヒットした時の待機状態
	void UpdateIdle3();
	// 待機2の終了待ち
	void UpdateIdleWait();

	// 攻撃
	void UpdateAttack();
	// 攻撃2
	void UpdateAttack2();
	// 攻撃終了待ち
	void UpdateAttackWait();

	//ヒット
	void UpdateHIt();
	// 死ぬ時
	void UpdateDie();
	// めまい(混乱)
	void UpdateDizzy();

	// 更新処理
	void Update();

	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	virtual void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 攻撃開始
	void AttackStart() override;
	// 攻撃終了
	void AttackEnd() override;

	// 描画
	void Render();

	// 1レベルアップ
	void LevelUp();
	// レベルの変更
	void ChangeLevel(int level);

	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	/// <param name="damage">受けるダメージ</param>
	//ダメージを与えたオブジェクト
	virtual void TakeDamage(int damage, CObjectBase*causedObj);

	static int mHp;

private:
	int mAttackTime;   // 攻撃時間の間隔
	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ
		eIdle,		// 待機
		eIdle2,		// 待機2
		eIdle3,		// 待機3
		eIdle4,		// 待機4
		eAttack,	// 攻撃
		eAttack2,	// 攻撃2
		eHit,       // ヒット
		eDie,       // 死ぬ時
		eDizzy,     // めまい(混乱)
		eWalk,		// 歩行
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// レッドスライム(エネミー)のインスタンス
	static CSlime* spInstance;

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// レッドスライム(エネミー)の状態
	enum class EState
	{
		eIdle,		// 待機
		eIdle2,     // 待機2
		eIdle3,     // 待機3
		eIdleWait,  // 待機終了待ち
		eAttack,	// 攻撃
		eAttack2,	// 攻撃2
		eAttackWait,// 攻撃終了待ち
		eHit,       // ヒット
		eDie,       // 死ぬ時
		eDizzy,     // めまい(混乱)
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了
	};
	EState mState;	// レッドスライム(エネミー)の状態

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphere;
	CColliderSphere* mpDamageCol;  // ダメージを受けるコライダー
	CColliderSphere* mpAttackCol;  // ダメージを与えるコライダー
	CTransform* mpRideObject;

	CHpGauge* mpHpGauge;  // HPゲージ
};
