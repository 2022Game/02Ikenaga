#pragma once
//キャラクタクラスのインクルード
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderSphere.h"
#include "CRideableObject.h"
#include "CImage.h"
class CHpGauge;
class CSaGauge;
class CSword;
class CShield;

#include <algorithm>

#define DEFAULT_CAMERA_POS CVector(0.0f, 50.0f, 75.0f)
/*
プレイヤークラス
キャラクタクラスを継承
*/
class CPlayer : public CXCharacter
{
public:
	bool IsDeath() const;
	bool IsDeath2() const;
	bool IsDeath3() const;

	// インスタンスのポインタの取得
	static CPlayer* Instance();

	// コンストラクタ
	CPlayer();
	// デストラクタ
	~CPlayer();

	// 待機状態
	void UpdateIdle();
	// 攻撃
	void UpdateAttack();
	// 攻撃2
	void UpdateAttack2();
	// 攻撃3
	void UpdateAttack3();
	// 攻撃4
	void UpdateAttack4();
	// 攻撃5
	void UpdateAttack5();
	// 攻撃6
	void UpdateAttack6();
	// 攻撃終了待ち
	void UpdateAttackWait();
	// ジャンプ開始
	void UpdateJumpStart();
	// ジャンプ中
	void UpdateJump();
	// ジャンプ終了
	void UpdateJumpEnd();
	//攻撃力アップ
	void UpdatePowerUp();
	// 攻撃力アップ終了
	void UpdatePowerUpEnd();
	// ヒット
	void UpdateHit();

	// 1レベルアップ
	void LevelUp();
	// レベルの変更
	virtual void ChangeLevel(int level);

	// HP回復と特殊攻撃(SA)回数を自動回復
	void AutomaticRecovery();

	// 経験値を加算
	void AddExp(int exp);

	// 更新
	void Update();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 描画
	void Render();

	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	/// <param name="damage">受けるダメージ</param>
	virtual void TakeDamage(int damage, CObjectBase* causedObj);

	static int mPower;
private:
	int mAttackTime;
	int mAttackcount;
	// 回復までのカウント
	int healcount;
	// 特殊攻撃の回数の回復までのカウント
	int recoverycount;
	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		//eTPose,		// Tポーズ
		eIdle,		// 待機
		eWalk,		// 歩行
		eAttack,	// 攻撃
		eJumpStart,	// ジャンプ開始
		eAttack2,	// 攻撃2
		eAttack3,	// 攻撃3
		eAttack4,	// 攻撃4
	    eAttack5,	// 攻撃5
		eAttack6,	// 攻撃6
		ePowerUp,   // 攻撃力アップ
		eHit,       // ヒット
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// プレイヤーのインスタンス
	static CPlayer* spInstance;

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// プレイヤーの状態
	enum class EState
	{
		eIdle,		// 待機
		eAttack,	// 攻撃
		eAttack2,	// 攻撃2
		eAttack3,	// 攻撃3
		eAttack4,	// 攻撃4
		eAttack5,	// 攻撃5
		eAttack6,	// 攻撃6
		eAttackWait,// 攻撃終了待ち
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了
		ePowerUp,   // 攻撃力アップ
		ePowerUpEnd,  // 攻撃力アップ終了
		eHit,       // ヒット
		eStop,      // 停止
	};
	EState mState;	// プレイヤーの状態

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphere;
	CColliderSphere* mpDamageCol;  // ダメージを受けるコライダー

	CTransform* mpRideObject;

	CHpGauge* mpHpGauge;  // HPゲージ
	CSaGauge* mpSaGauge;  // SAゲージ(スペシャルアタック)

	CVector mDefaultPos;  // デフォルトの座標

	CSword* mpSword;      // 右手に持つ剣
	CShield* mpShield;    // 左手に持つ盾
};
