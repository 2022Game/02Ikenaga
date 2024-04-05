#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

/*
 ボクサー2
 プレイヤーの推定レベル(81～90)
 エネミークラスを継承
*/
class CBoxer2 : public CEnemy
{
public:
	//インスタンスのポインタの取得
	static CBoxer2* Instance();

	// コンストラクタ
	CBoxer2();
	~CBoxer2();

	// 待機状態
	void UpdateIdle();
	// 待機状態2
	void UpdateIdle2();

	// 攻撃
	void UpdateAttack();
	// 攻撃2
	void UpdateAttack2();
	// 攻撃終了待ち
	void UpdateAttackWait();

	// ジャンプ開始
	void UpdateJumpStart();
	// ジャンプ中
	void UpdateJump();
	// ジャンプ終了
	void UpdateJumpEnd();

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
		eAttack,	  // 攻撃
		eAttack2,	  // 攻撃2
		eHit,         // ヒット
		eDefense,     // 防御
		eDefenseHit,  // 防御中のヒット
		eDie,         // 死ぬ
		eDizzy,       // めまい(混乱)
		eJumpStart,	  // ジャンプ開始
		eJump,		  // ジャンプ中
		eJumpEnd,	  // ジャンプ終了
		eRun,		  // 移動
		eSlide,       // 滑る

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// ボクサー2のインスタンス
	static CBoxer2* spInstance;

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// ボクサー2の状態
	enum class EState
	{
		eIdle,		  // 待機
		eIdle2,		  // 待機2
		eAttack,	  // 攻撃
		eAttack2,	  // 攻撃2
		eAttackWait,  // 攻撃終了待ち
		eHit,         // ヒット
		eDefense,     // 防御
		eDefenseHit,  // 防御中のヒット
		eDie,         // 死ぬ時
		eDizzy,       // めまい(混乱)
		eJumpStart,	  // ジャンプ開始
		eJump,		  // ジャンプ中
		eJumpEnd,	  // ジャンプ終了
		eRun,         // 移動
		eSlide,       // 滑る
	};
	EState mState;	// ボクサー2の状態

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphere;   // キャラクター押し戻しコライダー(頭)
	CColliderSphere* mpColliderSphere2;  // キャラクター押し戻しコライダー(体)
	CColliderSphere* mpColliderSphere3;  // キャラクター押し戻しコライダー(右手)
	CColliderSphere* mpColliderSphere4;  // キャラクター押し戻しコライダー(左手)
	CColliderSphere* mpColliderSphere5;  // キャラクター押し戻しコライダー(右足)
	CColliderSphere* mpColliderSphere6;  // キャラクター押し戻しコライダー(左足)

	CColliderSphere* mpDamageCol;   // ダメージを受けるコライダー(頭)
	CColliderSphere* mpDamageCol2;  // ダメージを受けるコライダー(体)
	CColliderSphere* mpDamageCol3;  // ダメージを受けるコライダー(右手)
	CColliderSphere* mpDamageCol4;  // ダメージを受けるコライダー(左手)
	CColliderSphere* mpDamageCol5;  // ダメージを受けるコライダー(右足)
	CColliderSphere* mpDamageCol6;  // ダメージを受けるコライダー(左足)

	CColliderSphere* mpAttackCol;   // ダメージを与えるコライダー(右手)
	CColliderSphere* mpAttackCol2;  // ダメージを与えるコライダー(右足)
	CColliderSphere* mpAttackCol3;  // ダメージを与えるコライダー(左足)
	CColliderSphere* mpAttackCol4;  // ダメージを与えるコライダー(頭)
	CColliderSphere* mpAttackCol5;  // ダメージを与えるコライダー(体)
	
	CTransform* mpRideObject;
};