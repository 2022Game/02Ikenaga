#pragma once
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"

class CHit;

/*
 マッシュルームクラス
 プレイヤーの推定レベル(6～10)
 エネミークラスを継承
*/
class CMushroom : public CEnemy
{
public:
	//インスタンスのポインタの取得
	static CMushroom* Instance();

	// コンストラクタ
	CMushroom();

	// デストラクタ
	~CMushroom();

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
	// 攻撃3
	void UpdateAttack3();
	// 攻撃終了待ち
	void UpdateAttackWait();

	//ヒット
	void UpdateHit();
	// 死ぬ時
	void UpdateDie();
	// めまい(混乱)
	void UpdateDizzy();
	// 移動
	void UpdateRun();

	int mAttackTime;   // 攻撃時間の間隔

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ
		eIdle,		// 戦う前の待機
		eIdle2,		// 戦う前の待機2
		eIdle3,     // 待機状態3
		eIdle4,     // 待機状態4
		eAttack,	// 攻撃
		eAttack2,	// 攻撃2
		eAttack3,	// 攻撃3
		eHit,       // ヒット
		eDie,       // 死ぬ
		eDizzy,     // めまい(混乱)
		eRun,		// 移動

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// マッシュルームのインスタンス
	static CMushroom* spInstance;

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

	// マッシュルームの状態
	enum class EState
	{
		eIdle,		  // 戦う前の待機
		eIdle2,		  // 戦う前の待機2
		eIdle3,       // 待機状態3
		eAttack,	  // 攻撃
		eAttack2,	  // 攻撃2
		eAttack3,	  // 攻撃3
		eAttackWait,  // 攻撃終了待ち
		eHit,         // ヒット
		eDie,         // 死ぬ時
		eDizzy,       // めまい(混乱)
		eRun,         // 移動
	};
	EState mState;	  // マッシュルームの状態
	int mStateStep;   // State内のステップ処理

	// 状態を切り替え
	void ChangeState(EState state);

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか

	 // 線分コライダー
	CColliderLine* mpColLineSide;           // 線分コライダー(横)
	CColliderLine* mpColLineHeight;         // 線分コライダー(高さ)

	// キャラクターの押し戻しコライダー
	CColliderSphere* mpColliderSphereHead;  // 頭
	CColliderSphere* mpColliderSphereBody;  // 体
	CColliderSphere* mpColliderSphereRoot;  // 根

	// ダメージを受けるコライダー
	CColliderCapsule* mpDamageColBody;      // 体
	CColliderSphere* mpDamageColUmbrella;   // かさ
	CColliderSphere* mpDamageColRoot;       // 根

	// ダメージを与えるコライダー
	CColliderSphere* mpAttackColHead;       // 頭
	CColliderSphere* mpAttackColRoot;       // 根
	CTransform* mpRideObject;

	CHit* mpHitEffect;  // ヒットエフェクト 
};