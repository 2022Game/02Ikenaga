#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"

class CHit;
class CDizzyEffect;

/*
 チェストモンスター
 プレイヤーの推定レベル(51～60)
 エネミークラスを継承
*/
class CChest : public CEnemy
{
public:
	// インスタンスのポインタの取得
	static CChest* Instance();

	// コンストラクタ
	CChest();
	// デストラクタ
	~CChest();

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
	// ダメージを与えたオブジェクト
	virtual void TakeDamage(int damage, CObjectBase* causedObj);

	// 死亡処理
	void Death() override;

	// ランダムに位置を取得
	CVector GetRandomSpawnPos()override;

	// 描画
	void Render();

private:

	// 戦う前の待機状態
	void UpdateIdle();
	// 戦う前の待機状態2
	void UpdateIdle2();
	// 待機状態3
	void UpdateIdle3();

	// 攻撃
	void UpdateAttack();
	// 攻撃2
	void UpdateAttack2();
	// 攻撃終了待ち
	void UpdateAttackWait();

	// ヒット
	void UpdateHit();
	// 死ぬ時
	void UpdateDie();
	// めまい(混乱)
	void UpdateDizzy();

	// 移動
	void UpdateRun();

	float mAttackTime;  // 次の攻撃時間

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ
		eIdle,		// 戦う前の待機
		eIdle2,		// 待機2
		eIdle3,     // 待機状態3
		eAttack,	// 攻撃
		eAttack2,	// 攻撃2
		eHit,       // ヒット
		eDie,       // 死ぬ
		eDizzy,     // めまい(混乱)
		eRun,		// 移動

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// チェストモンスターのインスタンス
	static CChest* spInstance;

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

	// チェストモンスターの状態
	enum class EState
	{
		eIdle,		  // 戦う前の待機
		eIdle2,		  // 戦う前の待機2
		eIdle3,       // 待機状態3
		eAttack,	  // 攻撃
		eAttack2,	  // 攻撃2
		eAttackWait,  // 攻撃終了待ち
		eHit,         // ヒット
		eDie,         // 死ぬ時
		eDizzy,       // めまい(混乱)
		eRun          // 移動
	};
	EState mState;	  // チェストモンスターの状態

	int mStateStep;   // State内のステップ処理

	// 状態を切り替え
	void ChangeState(EState state);

	CVector mMoveSpeed;  // 移動速度
	bool mIsGrounded;	 // 接地しているかどうか
	bool mIsSpawnedCoinEffect;  // コイン発射

	// 線分コライダー
	CColliderLine* mpColLineSide;    // 線分コライダー(横)
	CColliderLine* mpColLineHeight;  // 線分コライダー(縦)

	// キャラクターの押し戻しコライダー
	CColliderSphere* mpColliderSphereHead;   // 頭
	CColliderSphere* mpColliderSphereBody;   // 体
	CColliderSphere* mpColliderSphereFeet;   // 前の左足
	CColliderSphere* mpColliderSphereFeet2;  // 前の右足

	// ダメージを受けるコライダー
	CColliderSphere* mpDamageColHead;        // 頭
	CColliderSphere* mpDamageColBody;        // 体
	CColliderCapsule* mpDamageColFeet;       // 前の左足
	CColliderCapsule* mpDamageColFeet2;      // 前の右足

	// ダメージを与えるコライダー
	CColliderSphere* mpAttackColHead;        // 頭
	CTransform* mpRideObject;

	// エフェクト関連
	CHit* mpHitEffect;  // ヒットエフェクト 
	// めまいのエフェクト
	CDizzyEffect* mpDizzyEffect;
};