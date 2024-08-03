#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

class CImpactEffect;
class CShieldRotate2;
class CHit;

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

	// ランダムに位置を取得
	CVector GetRandomSpawnPos()override;

	// 描画
	void Render();

private:

	float mDefenseTime;  // 次の防御時間
	float mAttackTime;   // 次の攻撃時間

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

	int mStateStep;  // State内のステップ処理

	// 状態を切り替え
	void ChangeState(EState state);

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか

	// 線分コライダー
	CColliderLine* mpColLineSide;    // 線分コライダー(横)
	CColliderLine* mpColLineHeight;  // 線分コライダー(縦)

	// キャラクター押し戻しコライダー
	CColliderSphere* mpColliderSphereHead;   // 頭
	CColliderSphere* mpColliderSphereBody;   // 体
	CColliderSphere* mpColliderSphereHandR;  // 右手
	CColliderSphere* mpColliderSphereHandL;  // 左手
	CColliderSphere* mpColliderSphereFeetR;  // 右足
	CColliderSphere* mpColliderSphereFeetL;  // 左足

	// ダメージを受けるコライダー
	CColliderSphere* mpDamageColHead;        // 頭
	CColliderSphere* mpDamageColBody;        // 体
	CColliderSphere* mpDamageColHandR;       // 右手
	CColliderSphere* mpDamageColHandL;       // 左手
	CColliderSphere* mpDamageColArmR;        // 右腕
	CColliderSphere* mpDamageColArmL;        // 左腕
	CColliderSphere* mpDamageColFeetR;       // 右足
	CColliderSphere* mpDamageColFeetL;       // 左足

	// ダメージを与えるコライダー
	CColliderSphere* mpAttackColHead;         // 頭
	CColliderSphere* mpAttackColBody;         // 体
	CColliderSphere* mpAttackColHandR;        // 右手
	CColliderSphere* mpAttackColFeetR;        // 右足
	CColliderSphere* mpAttackColFeetL;        // 左足

	CTransform* mpRideObject;

	// 回転するシールド
	CShieldRotate2* mpShieldRotate;
	CShieldRotate2* mpShieldRotate2;
	CShieldRotate2* mpShieldRotate3;
	CShieldRotate2* mpShieldRotate4;

	// エフェクト関連
	CImpactEffect* mpImpact;  // 衝撃エフェクト
	CHit* mpHitEffect;        // ヒットエフェクト
};