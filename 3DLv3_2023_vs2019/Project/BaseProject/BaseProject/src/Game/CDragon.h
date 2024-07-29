#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"
class CFlamethrower;
class CFlightFlamethrower;
class CHit;

/*
 ボスドラゴン
 プレイヤーの推定レベル(91～100)
 エネミークラスを継承
*/
class CDragon : public CEnemy
{
public:
	//インスタンスのポインタの取得
	static CDragon* Instance();

	// コンストラクタ
	CDragon();
	// デストラクタ
	~CDragon();

	// 待機状態
	void UpdateIdle();
	// 待機状態2
	void UpdateIdle2();
	// 待機状態3
	void UpdateIdle3();

	// 攻撃(火炎放射)
	void UpdateAttack();
	// 攻撃2(前に飛んで後ろに下がる)
	void UpdateAttack2();
	// 攻撃3(嚙みつき)
	void UpdateAttack3();
	// 攻撃終了待ち
	void UpdateAttackWait();
	// 飛行時の攻撃
	void UpdateFlyingAttack();
	// 飛行時の攻撃終了待ち
	void UpdateFlyingAttackWait();

	//ヒット
	void UpdateHit();
	// 防御
	void UpdateDefense();
	// 死ぬ時
	void UpdateDie();

	// 雄叫び
	void UpdateRoar();

	// 飛行開始
	void UpdateFlyingStart();
	// 飛行の待機
	void UpdateFlyingIdle();
	// 飛行終了
	void UpdateFlyingEnd();

	// 移動
	void UpdateRun();

	// 飛行中の移動
	void UpdateFlyForward();

	// バックステップ
	void UpdateBackStep();

	// 更新処理
	void Update();

	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

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
	virtual void TakeDamage(int damage, CObjectBase* causedObj);

	/// <summary>
	/// 防御力の強化割合を取得
	/// </summary>
	/// <param name="attackDir"></param>
	/// <returns></returns>
	float GetDefBuff(const CVector& attackDir) const override;

	// 死亡処理
	void Death() override;

private:
	int mFlyingTime;        // 飛行時間
	int mDefenseTime;       // 防御時間の間隔
	int mAttackTime;        // 攻撃時間の間隔
	int mFlyingAttackTime;  // 飛行時の攻撃時間の間隔
	float mBackStepTime;    // バックステップするまでの時間
	bool mRoarCount;        // 雄叫びのカウント

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		   // Tポーズ
		eIdle,		   // 待機
		eIdle2,		   // 待機2
		eIdle3,        // 待機状態3
		eAttack,	   // 攻撃(火炎放射)
		eAttack2,	   // 攻撃2(前に飛んで後ろに下がる)
		eAttack3,	   // 攻撃3(嚙みつき)
		eFlyingAttack, // 飛行時の攻撃
		eHit,          // ヒットWait
		eDefense,      // 防御
		eDie,          // 死ぬ
		eRoar,         // 雄叫び
		eFlyingStart,  // 飛行開始
		eFlyingIdle,   // 飛行の待機
		eFlyingEnd,    // 飛行終了
		eRun,		   // 移動
		eFlyForward,   // 飛行中の移動
		eBackStep,     // バックステップ

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// 雄叫びのエフェクトを作成
	void CreateRoar();

	// ドラゴンのインスタンス
	static CDragon* spInstance;

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

	// ドラゴンの状態
	enum class EState
	{
		eIdle,		       // 待機
		eIdle2,		       // 待機2
		eIdle3,            // 待機状態3
		eAttack,	       // 攻撃(火炎放射)
		eAttack2,	       // 攻撃2(前に飛んで後ろに下がる)
		eAttack3,	       // 攻撃3(嚙みつき)
		eAttackWait,       // 攻撃終了待ち
		eFlyingAttack,     // 飛行時の攻撃(火炎放射)
		eFlyingAttackWait, // 飛行時の攻撃終了待ち
		eHit,              // ヒット
		eDefense,          // 防御
		eDie,              // 死ぬ時
		eRoar,             // 雄叫び
		eFlyingStart,      // 飛行開始
		eFlyingIdle,       // 飛行の待機
		eFlyingEnd,        // 飛行終了
		eRun,              // 移動
		eFlyForward,       // 飛行中の移動
		eBackStep,         // バックステップ
	};
	EState mState;	 // ドラゴンの状態
	int mStateStep;  // State内のステップ処理

	// 状態を切り替え
	void ChangeState(EState state);

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか

	 // 線分コライダー
	CColliderLine* mpColliderLine;

	// キャラクター押し戻しコライダー
	CColliderCapsule* mpColSphereMouth;  // 口
	CColliderCapsule* mpColSphereNeck;   // 首
	CColliderSphere* mpColSphereBody;    // 体
	CColliderSphere* mpColSphereFeet;    // 前の左足
	CColliderSphere* mpColSphereFeet2;   // 前の右足
									     
	// ダメージを受けるコライダー	   	 
	CColliderCapsule* mpDamageColMouth;  // 口
	CColliderCapsule* mpDamageColNeck;   // 首
	CColliderSphere* mpDamageColChest;   // 胸
	CColliderSphere* mpDamageColFeet;    // 前の左足
	CColliderSphere* mpDamageColFeet2;   // 前の右足
	CColliderSphere* mpDamageColFeet3;   // 後ろの左足
	CColliderSphere* mpDamageColFeet4;   // 後ろの右足
									     
	// ダメージを与えるコライダー	   	 
	CColliderCapsule* mpAttackColMouth;  // 口
	CColliderSphere* mpAttackColBody;    // 体
	CColliderSphere* mpAttackColFeet;    // 前の左足
	CColliderSphere* mpAttackColFeet2;   // 前の右足

	CTransform* mpRideObject;

	// 火炎放射エフェクト
	CFlamethrower* mpFlamethrower;
	// 飛行中の火炎放射エフェクト
	CFlightFlamethrower* mpFlightFlamethrower;

	// 波動エフェクト
	bool mIsSpawnedRoarEffect;
	// 経過時間計測用(雄叫び)
	float mElapsedRoarTime;
	CHit* mpHitEffect;  // ヒットエフェクト 
};