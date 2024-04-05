#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

/*
 亀2
 プレイヤーの推定レベル(11～20)
 エネミークラスを継承
*/
class CTurtle2 : public CEnemy
{
public:
	//インスタンスのポインタの取得
	static CTurtle2* Instance();

	// コンストラクタ
	CTurtle2();
	~CTurtle2();

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

	static int mHp;

private:
	// 待機状態
	void UpdateIdle();
	// 待機状態2
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
	// 防御
	void UpdateDefense();
	// 防御中のヒット
	void UpdateDefenseHit();
	// 防御中の待機
	void UpdateDefenseIdle();
	// 死ぬ時
	void UpdateDie();
	// めまい(混乱)
	void UpdateDizzy();
	// 走る
	void UpdateRun();

	int mDefenseTime;  // 防御時間の間隔
	int mAttackTime;   // 攻撃時間の間隔
	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		  // Tポーズ
		eIdle,		  // 待機
		eIdle2,		  // 待機2
		eIdle3,       // 待機3
		eIdle4,       // 待機4
		eAttack,	  // 攻撃
		eAttack2,	  // 攻撃2
		eHit,         // ヒット
		eDefense,     // 防御
		eDefenseHit,  // 防御中のヒット
		eDefenseIdle, // 防御中の待機
		eDie,         // 死ぬ
		eDizzy,       // めまい(混乱)
		eRun,		  // 走る

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// 亀2のインスタンス
	static CTurtle2* spInstance;

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// 亀2の状態
	enum class EState
	{
		eIdle,		  // 待機
		eIdle2,		  // 待機2
		eIdle3,       // 待機3
		eAttack,	  // 攻撃
		eAttack2,	  // 攻撃2
		eAttackWait,  // 攻撃終了待ち
		eHit,         // ヒット
		eDefense,     // 防御
		eDefenseHit,  // 防御中のヒット
		eDefenseIdle, // 防御中の待機
		eDie,         // 死ぬ時
		eDizzy,       // めまい(混乱)
		eRun,         // 走る
	};
	EState mState;	  // 亀2の状態

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;   // 接地しているかどうか

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphere;  // キャラクター押し戻しコライダー
	CColliderSphere* mpDamageCol;  // ダメージを受けるコライダー
	CColliderSphere* mpAttackCol;  // ダメージを与えるコライダー
	CTransform* mpRideObject;
};