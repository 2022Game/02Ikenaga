#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

class CCrackEffect;

/*
 サボテン3
 プレイヤーの推定レベル(50)
 エネミークラスを継承
*/
class CCactus3 : public CEnemy
{
public:
	//インスタンスのポインタの取得
	static CCactus3* Instance();

	// コンストラクタ
	CCactus3();
	~CCactus3();

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

	// サボテン3のインスタンス
	static CCactus3* spInstance;

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// サボテン3の状態
	enum class EState
	{
		eIdle,		// 戦う前の待機
		eIdle2,		// 戦う前の待機2
		eIdle3,     // 待機状態3
		eAttack,	// 攻撃
		eAttack2,	// 攻撃2
		eAttackWait,// 攻撃終了待ち
		eHit,       // ヒット
		eDie,       // 死ぬ時
		eDizzy,     // めまい(混乱)
		eRun,       // 移動
	};
	EState mState;	// サボテン3の状態

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphereHead;      // キャラクター押し戻しコライダー(頭)
	CColliderSphere* mpColliderSphereBody;      // キャラクター押し戻しコライダー(体)
	CColliderSphere* mpColliderSphereFeet;      // キャラクター押し戻しコライダー(足元)
	CColliderSphere* mpColliderSphereLeftHand;  // キャラクター押し戻しコライダー(左手)
	CColliderSphere* mpColliderSphereRightHand; // キャラクター押し戻しコライダー(右手)

	CColliderSphere* mpDamageColHead;       // ダメージを受けるコライダー(頭)
	CColliderSphere* mpDamageColBody;       // ダメージを受けるコライダー(体)
	CColliderSphere* mpDamageColFeet;       // ダメージを受けるコライダー(足元)
	CColliderSphere* mpDamageColLeftHand;   // ダメージを受けるコライダー(左手)
	CColliderSphere* mpDamageColRightHand;  // ダメージを受けるコライダー(右手)

	CColliderSphere* mpAttackColHead;      // ダメージを与えるコライダー(頭)
	CColliderSphere* mpAttackColLeftHand;  // ダメージを与えるコライダー(左手)

	CTransform* mpRideObject;

	CCrackEffect* mpCrack;  // ひび割れ
};