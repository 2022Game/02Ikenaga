#pragma once
//キャラクタクラスのインクルード
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderSphere.h"
#include "CRideableObject.h"
#include "CImage.h"
#include "CSound.h"
#include "CText.h"

class CSword;
class CShield;
class CSlash;
class CShieldRotate;
class CHealCircle;
class CBuffCircle;
class CBuffAura;
class CPowerUpAura;
class CGamePlayerUI;

#include <algorithm>

#define DEFAULT_CAMERA_POS CVector(0.0f, 10.0f, 50.0f)

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

	// 死ぬ
	void UpdateDei();

	// 復活
	void UpdateRevival();

	// 1レベルアップ
	void LevelUp();
	// レベルの変更
	virtual void ChangeLevel(int level);

	// HP回復と特殊攻撃(SA)回数を自動回復
	void AutomaticRecovery();
	// 攻撃が当たったら特殊攻撃(SA)を回復
	virtual void AttackRecovery();

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
	static int mMaxHp;
	static int mSp;
	static int mRecoveryCount;  // 特殊攻撃の回数の回復までのカウント
	static bool mHeal;
	static bool mRolling;

private:
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
	// ジャンプ攻撃
	void UpdateJumpAttack();
	// ジャンプ攻撃終了待ち
	void UpdateJumpAttackWait();

	// 回避回数を増やす時間
	int mRollingTime;
	// 回避回数
	int mRollingCount;
	// 攻撃時間
	int mAttackTime;
	//	攻撃回数
	int mAttackCount;
	// 回復までのカウント
	int mHealCount;
	// 回復量
	int mRecoveryAmount;

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

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
		eJumpAttack,// ジャンプ攻撃
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
		float animSpeed;    // アニメーションの再生速度
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// プレイヤーの状態
	enum class EState
	{
		eIdle,		      // 待機
		eWalk,            // 歩行
		eAttack,	      // 攻撃
		eAttack2,	      // 攻撃2
		eAttack3,	      // 攻撃3
		eAttack4,	      // 攻撃4
		eAttack5,	      // 攻撃5
		eAttack6,	      // 攻撃6
		eAttack7,	      // 攻撃7
		eAttackWait,      // 攻撃終了待ち
		eJumpStart,	      // ジャンプ開始
		eJump,		      // ジャンプ中
		eJumpEnd,	      // ジャンプ終了
		ePowerUp,         // 攻撃力アップ
		ePowerUpEnd,      // 攻撃力アップ終了
		eHit,             // ヒット
		eGuard,           // ガード
		eGuardHit,        // ガードヒット
		eRolling,         // 回避
		eDie,             // 死ぬ
		eJumpAttack,      // ジャンプ攻撃
		eJumpAttackWait,  // ジャンプ攻撃終了待ち
	};
	// プレイヤーの状態
	EState mState;
	int mStateStep;
	int mStateJumpAttackStep;
	int mBuffStep;

	// 状態を切り替え
	void ChangeState(EState state);

	CVector mMoveSpeed;      // 移動速度
	bool mIsGrounded;        // 接地しているかどうか

	// 線分コライダー
	CColliderLine* mpColliderLine;
	// キャラクターの押し戻しコライダー(頭)
	CColliderSphere* mpColliderSphereHead;
	// キャラクターの押し戻しコライダー(体)
	CColliderSphere* mpColliderSphereBody;

	// ダメージを受けるコライダー(頭)
	CColliderSphere* mpDamageColHead;
	// ダメージを受けるコライダー(体)
	CColliderSphere* mpDamageColBody;

	CTransform* mpRideObject;

	CVector mDefaultPos;  // デフォルトの座標
	CVector current;

	// 武器関連
	CSword* mpSword;      // 右手に持つ剣
	CShield* mpShield;    // 左手に持つ盾
	CSlash* mpSlash;      // スラッシュ

	// 回転するシールド
	CShieldRotate* mpShieldRotate;
	CShieldRotate* mpShieldRotate2;
	CShieldRotate* mpShieldRotate3;
	CShieldRotate* mpShieldRotate4;

	// エフェクト関連
	CHealCircle* mpHealCircle;    // 回復用のサークル
	CBuffCircle* mpBuffCircle;    // バフサークル
	CBuffAura* mpBuffAura;        // バフオーラ
	CPowerUpAura* mpPowerUpAura;  // パワーアップオーラ

	// SE関連
	CSound* mpSlashSE;            // スラッシュの効果音
	bool mIsPlayedSlashSE;
	bool mIsSpawnedSlashEffect;

	CGamePlayerUI* mpGameUI;
};
