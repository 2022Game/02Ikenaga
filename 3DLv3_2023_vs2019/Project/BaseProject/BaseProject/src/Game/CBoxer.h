#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

class CImpactEffect;

/*
 ボクサー
 プレイヤーの推定レベル(71～80)
 エネミークラスを継承
*/
class CBoxer : public CEnemy
{
public:
	//インスタンスのポインタの取得
	static CBoxer* Instance();

	// コンストラクタ
	CBoxer();
	// デストラクタ
	~CBoxer();

	// 待機状態
	void UpdateIdle();
	// 待機状態2
	void UpdateIdle2();

	// 攻撃(パンチ)
	void UpdateAttack();
	// 攻撃2(蹴り)
	void UpdateAttack2();
	// 攻撃終了待ち
	void UpdateAttackWait();

	//ヒット
	void UpdateHit();
	// 防御
	void UpdateDefense();
	// 防御中のヒット
	void UpdateDefenseHit();

	// 死ぬ時
	void UpdateDie();
	// めまい(混乱)
	void UpdateDizzy();

	// ジャンプ中
	void UpdateJump();
	// ジャンプ終了
	void UpdateJumpEnd();

	// 移動
	void UpdateRun();
	// 滑る
	void UpdateSlide();

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
	int mDefenseTime;  // 防御時間の間隔
	int mAttackTime;   // 攻撃時間の間隔

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		  // Tポーズ
		eIdle,		  // 待機
		eIdle2,		  // 待機2
		eAttack,	  // 攻撃(パンチ)
		eAttack2,	  // 攻撃2(蹴り)
		eHit,         // ヒット
		eDefense,     // 防御
		eDefenseHit,  // 防御中のヒット
		eDie,         // 死ぬ
		eDizzy,       // めまい(混乱)
		eJump,		  // ジャンプ中
		eJumpEnd,	  // ジャンプ終了
		eRun,		  // 移動
		eSlide,       // 滑る

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// ボクサーのインスタンス
	static CBoxer* spInstance;

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

	// ボクサーの状態
	enum class EState
	{
		eIdle,		  // 待機
		eIdle2,		  // 待機2
		eAttack,	  // 攻撃(パンチ)
		eAttack2,	  // 攻撃2(蹴り)
		eAttackWait,  // 攻撃終了待ち
		eHit,         // ヒット
		eDefense,     // 防御
		eDefenseHit,  // 防御中のヒット
		eDie,         // 死ぬ時
		eDizzy,       // めまい(混乱)
		eJump,		  // ジャンプ中
		eJumpEnd,	  // ジャンプ終了
		eRun,         // 移動
		eSlide,       // 滑る
	};
	EState mState;	// ボクサーの状態

	int mStateAttackStep;     // State内の攻撃のステップ処理
	int mStateAttack2Step;    // State内の攻撃2のステップ処理
	int mStateSlideStep;      // State内の滑るステップ処理

	// 状態を切り替え
	void ChangeState(EState state);

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか

	CColliderLine* mpColliderLine;

	CColliderSphere* mpColliderSphereHead;    // キャラクター押し戻しコライダー(頭)
	CColliderSphere* mpColliderSphereBody;    // キャラクター押し戻しコライダー(体)
	CColliderSphere* mpColliderSphereHandR;   // キャラクター押し戻しコライダー(右手)
	CColliderSphere* mpColliderSphereHandL;   // キャラクター押し戻しコライダー(左手)
	CColliderSphere* mpColliderSphereFeetR;   // キャラクター押し戻しコライダー(右足)
	CColliderSphere* mpColliderSphereFeetL;   // キャラクター押し戻しコライダー(左足)

	CColliderSphere* mpDamageColHead;         // ダメージを受けるコライダー(頭)
	CColliderSphere* mpDamageColBody;         // ダメージを受けるコライダー(体)
	CColliderSphere* mpDamageColHandR;        // ダメージを受けるコライダー(右手)
	CColliderSphere* mpDamageColHandL;        // ダメージを受けるコライダー(左手)
	CColliderSphere* mpDamageColFeetR;        // ダメージを受けるコライダー(右足)
	CColliderSphere* mpDamageColFeetL;        // ダメージを受けるコライダー(左足)

	CColliderSphere* mpAttackColHead;         // ダメージを与えるコライダー(頭)
	CColliderSphere* mpAttackColBody;         // ダメージを与えるコライダー(体)
	CColliderSphere* mpAttackColHandR;        // ダメージを与えるコライダー(右手)
	CColliderSphere* mpAttackColFeetR;        // ダメージを与えるコライダー(右足)
	CColliderSphere* mpAttackColFeetL;        // ダメージを与えるコライダー(左足)

	CTransform* mpRideObject;

	CImpactEffect* mpImpact;
};