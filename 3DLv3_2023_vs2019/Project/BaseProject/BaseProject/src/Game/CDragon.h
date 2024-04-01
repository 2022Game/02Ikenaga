#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"
class CFlamethrower;

/*
 ボスドラゴン
 エネミークラスを継承
*/
class CDragon : public CEnemy
{
public:
	//インスタンスのポインタの取得
	static CDragon* Instance();

	// コンストラクタ
	CDragon();
	~CDragon();

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
	// 攻撃3
	void UpdateAttack3();
	// 攻撃終了待ち
	void UpdateAttackWait();

	//ヒット
	void UpdateHit();
	// 防御
	void UpdateDefense();
	// 死ぬ時
	void UpdateDie();

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
		eIdle3,       // 待機状態3
		eAttack,	  // 攻撃
		eAttack2,	  // 攻撃2
		eAttack3,	  // 攻撃3
		eHit,         // ヒット
		eDefense,     // 防御
		eDie,         // 死ぬ
		eWalk,		  // 歩行
		eJumpStart,	  // ジャンプ開始
		eJump,		  // ジャンプ中
		eJumpEnd,	  // ジャンプ終了

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// ドラゴンのインスタンス
	static CDragon* spInstance;

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// ドラゴンの状態
	enum class EState
	{
		eIdle,		  // 待機
		eIdle2,		  // 待機2
		eIdle3,       // 待機状態3
		eAttack,	  // 攻撃
		eAttack2,	  // 攻撃2
		eAttack3,	  // 攻撃3
		eAttackWait,  // 攻撃終了待ち
		eHit,         // ヒット
		eDefense,     // 防御
		eDie,         // 死ぬ時
		eJumpStart,	  // ジャンプ開始
		eJump,		  // ジャンプ中
		eJumpEnd,	  // ジャンプ終了
	};
	EState mState;	// ドラゴンの状態

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか

	//CColliderLine* mpColliderLine;
	//CColliderSphere* mpColliderSphere;
	CColliderSphere* mpDamageCol;    // ダメージを受けるコライダー(頭)
	CColliderSphere* mpDamageCol2;   // ダメージを受けるコライダー(口)
	CColliderSphere* mpDamageCol3;   // ダメージを受けるコライダー(口の先端)
	CColliderSphere* mpDamageCol4;   // ダメージを受けるコライダー(首)
	CColliderSphere* mpDamageCol5;   // ダメージを受けるコライダー(首2)
	CColliderSphere* mpDamageCol6;   // ダメージを受けるコライダー(首3)
	CColliderSphere* mpDamageCol7;   // ダメージを受けるコライダー(胸)
	CColliderSphere* mpDamageCol8;   // ダメージを受けるコライダー(脊椎)
	CColliderSphere* mpDamageCol9;   // ダメージを受けるコライダー(脊椎2)
	CColliderSphere* mpDamageCol10;  // ダメージを受けるコライダー(つなぎ)
	CColliderSphere* mpDamageCol11;  // ダメージを受けるコライダー(つなぎ2)
	CColliderSphere* mpDamageCol12;  // ダメージを受けるコライダー(つなぎ3)
	CColliderSphere* mpDamageCol13;  // ダメージを受けるコライダー(つなぎ4)
	CColliderSphere* mpDamageCol14;  // ダメージを受けるコライダー(つなぎ5)
	CColliderSphere* mpDamageCol15;  // ダメージを受けるコライダー(前の左足)
	CColliderSphere* mpDamageCol16;  // ダメージを受けるコライダー(前の左足2)

	//CColliderSphere* mpAttackCol;  // ダメージを与えるコライダー
	CTransform* mpRideObject;

	// 火炎放射エフェクト
	CFlamethrower* mpFlamethrower;
};