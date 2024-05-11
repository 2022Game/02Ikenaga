#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"

class CCrackEffect;

/*
 サボテン
 プレイヤーの推定レベル(41～50)
 エネミークラスを継承
*/
class CCactus : public CEnemy
{
public:
	//インスタンスのポインタの取得
	static CCactus* Instance();

	// コンストラクタ
	CCactus();
	// デストラクタ
	~CCactus();

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

	//ヒット
	void UpdateHit();
	// 死ぬ時
	void UpdateDie();
	// めまい(混乱)
	void UpdateDizzy();
	// 移動
	void UpdateRun();

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

	// 死亡処理
	void Death() override;

private:
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
		eHit,       // ヒット
		eDie,       // 死ぬ
		eDizzy,     // めまい(混乱)
		eRun,		// 移動

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// サボテンのインスタンス
	static CCactus* spInstance;

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

	// サボテンの状態
	enum class EState
	{
		eHit,         // ヒット
		eIdle,		  // 戦う前の待機
		eIdle2,		  // 戦う前の待機2
		eIdle3,       // 待機状態3
		eAttack,	  // 攻撃
		eAttack2,	  // 攻撃2
		eAttackWait,  // 攻撃終了待ち
		eDie,         // 死ぬ時
		eDizzy,       // めまい(混乱)
		eRun,         // 移動
	};				  
	EState mState;	  // サボテンの状態

	int mStateAttackStep;   // State内の攻撃でのステップ処理
	int mStateAttack2Step;  // State内の攻撃2でのステップ処理

	// 状態を切り替え
	void ChangeState(EState state);

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか

	CColliderLine* mpColliderLine;              // キャラクターの線分コライダー
	CColliderSphere* mpColliderSphereHead;      // キャラクターの押し戻しコライダー(頭)
	CColliderSphere* mpColliderSphereBody;      // キャラクターの押し戻しコライダー(体)
	CColliderSphere* mpColliderSphereFeet;      // キャラクターの押し戻しコライダー(足元)
	CColliderSphere* mpColliderSphereLeftHand;  // キャラクターの押し戻しコライダー(左手)
	CColliderSphere* mpColliderSphereRightHand; // キャラクターの押し戻しコライダー(右手)

	CColliderSphere* mpDamageColHead;           // ダメージを受けるコライダー(頭)
	CColliderSphere* mpDamageColBody;           // ダメージを受けるコライダー(体)
	CColliderSphere* mpDamageColFeet;           // ダメージを受けるコライダー(足元)
	CColliderCapsule* mpDamageColLeftHand;      // ダメージを受けるコライダー(左手)
	CColliderCapsule* mpDamageColRightHand;     // ダメージを受けるコライダー(右手)

	CColliderSphere* mpAttackColHead;           // ダメージを与えるコライダー(頭)
	CColliderSphere* mpAttackColLeftHand;       // ダメージを与えるコライダー(左手)

	CTransform* mpRideObject;

	CCrackEffect* mpCrack;  // ひび割れ
};
