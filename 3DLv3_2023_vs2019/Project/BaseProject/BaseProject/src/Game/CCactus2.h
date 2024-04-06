#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

/*
 サボテン2
 プレイヤーの推定レベル(41～50)
 エネミークラスを継承
*/
class CCactus2 : public CEnemy
{
public:
	//インスタンスのポインタの取得
	static CCactus2* Instance();

	// コンストラクタ
	CCactus2();
	~CCactus2();

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

	// サボテン2のインスタンス
	static CCactus2* spInstance;

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// サボテン2の状態
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
	EState mState;	// サボテン2の状態

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphere;   // キャラクター押し戻しコライダー (頭)
	CColliderSphere* mpColliderSphere2;  // キャラクター押し戻しコライダー (体)
	CColliderSphere* mpColliderSphere3;  // キャラクター押し戻しコライダー (足元)
	CColliderSphere* mpColliderSphere4;  // キャラクター押し戻しコライダー (左手)
	CColliderSphere* mpColliderSphere5;  // キャラクター押し戻しコライダー (右手)

	CColliderSphere* mpDamageCol;   // ダメージを受けるコライダー (頭)
	CColliderSphere* mpDamageCol2;  // ダメージを受けるコライダー (体)
	CColliderSphere* mpDamageCol3;  // ダメージを受けるコライダー (足元)
	CColliderSphere* mpDamageCol4;  // ダメージを受けるコライダー (左手)
	CColliderSphere* mpDamageCol5;  // ダメージを受けるコライダー (右手)

	CColliderSphere* mpAttackCol;   // ダメージを与えるコライダー (頭)
	CColliderSphere* mpAttackCol2;  // ダメージを与えるコライダー2 (左手)

	CTransform* mpRideObject;
};