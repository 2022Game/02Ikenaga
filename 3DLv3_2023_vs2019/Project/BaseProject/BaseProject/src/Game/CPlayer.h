#pragma once
//キャラクタクラスのインクルード
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderSphere.h"
#include "CRideableObject.h"
#include "CImage.h"
#include "CSound.h"
class CHpGauge;
class CSaGauge;
class CSword;
class CShield;
class CBeamEffect;

#include <algorithm>

#define DEFAULT_CAMERA_POS CVector(0.0f, 10.0f, 35.0f)
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

	// 1レベルアップ
	void LevelUp();
	// レベルの変更
	virtual void ChangeLevel(int level);

	// HP回復と特殊攻撃(SA)回数を自動回復
	void AutomaticRecovery();
	// 攻撃が当たったら特殊攻撃(SA)を回復
	void AttackRecovery();

	//回避カウント	
	void RollingCount();

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


	/// <summary>
	/// 攻撃力の強化割合を取得
	/// </summary>
	/// <returns></returns>
	float GetAtkBuff() const override;

    /// <summary>
    /// 防御力の強化割合を取得
    /// </summary>
    /// <param name="attackDir"></param>
    /// <returns></returns>
    float GetDefBuff(const CVector& attackDir) const override;

	static int mHp;
	static int mSa;

private:
	// 待機状態
	void UpdateIdle();
	// 歩行
	void UpadateWalk();
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
	// 攻撃7
	void UpdateAttack7();
	// 攻撃終了待ち
	void UpdateAttackWait();
	// ジャンプ開始
	void UpdateJumpStart();
	// ジャンプ中
	void UpdateJump();
	// ジャンプ終了
	void UpdateJumpEnd();
	// 移動の更新処理
	void UpdateMove();
	// 攻撃力アップ
	void UpdatePowerUp();
	// 攻撃力アップ終了
	void UpdatePowerUpEnd();
	// ヒット
	void UpdateHit();
	// ガード
	void UpdateGuard();
	// ガード中のヒット
	void UpdateGuardHit();
	// 回避
	void UpdateRolling();
	// 死ぬ
	void UpdateDei();

	// 回避回数を増やす時間
	int mRollingTime;
	// 回避回数
	int mRollingCount;
	// 攻撃時間
	int mAttackTime;
	//	攻撃回数
	int mAttackCount;
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
		eAttack7,	// 攻撃7
		ePowerUp,   // 攻撃力アップ
		eHit,       // ヒット
		eGuard,     // ガード
		eGuardHit,  // ガードヒット
		eRolling,   // 回避
		eDie,       // 死ぬ
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
		eWalk,      // 歩行
		eAttack,	// 攻撃
		eAttack2,	// 攻撃2
		eAttack3,	// 攻撃3
		eAttack4,	// 攻撃4
		eAttack5,	// 攻撃5
		eAttack6,	// 攻撃6
		eAttack7,	// 攻撃7
		eAttackWait,// 攻撃終了待ち
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了
		ePowerUp,   // 攻撃力アップ
		ePowerUpEnd,  // 攻撃力アップ終了
		eHit,       // ヒット
		eGuard,     // ガード
		eGuardHit,  // ガードヒット
		eRolling,   // 回避
		eDie,       // 死ぬ
		eStop,      // 停止
	};
	EState mState;	// プレイヤーの状態

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphere;
	CColliderSphere* mpDamageCol;   // ダメージを受けるコライダー
	CColliderSphere* mpDamageCol2;  // ダメージを受けるコライダー

	CTransform* mpRideObject;

	CHpGauge* mpHpGauge;  // HPゲージ
	CSaGauge* mpSaGauge;  // SAゲージ(スペシャルアタック)

	CVector mDefaultPos;  // デフォルトの座標
	CVector current;

	CSword* mpSword;      // 右手に持つ剣
	CShield* mpShield;    // 左手に持つ盾

	CSound* mpSlashSE;
	bool mIsPlayedSlashSE;
	bool mIsSpawnedSlashEffect;
	
	CBeamEffect* mpBeam;
};
