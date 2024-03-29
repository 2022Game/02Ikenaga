#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

/*
 球体のモンスター
 エネミークラスを継承
*/
class CBeholder : public CEnemy
{
public:
	//インスタンスのポインタの取得
	static CBeholder* Instance();

	// コンストラクタ
	CBeholder();
	~CBeholder();

	// 戦う前の待機状態
	void UpdateIdle();
	// 戦う前の待機状態2
	void UpdateIdle2();

	// 攻撃
	void UpdateAttack();
	// 攻撃2
	void UpdateAttack2();
	// 攻撃3
	void UpdateAttack3();
	// 攻撃4
	void UpdateAttack4();
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
		eIdle,		// 待機
		eIdle2,		// 待機2
		eAttack,	// 攻撃
		eAttack2,	// 攻撃2
		eAttack3,	// 攻撃3
		eAttack4,	// 攻撃4
		eHit,       // ヒット
		eDie,       // 死ぬ
		eDizzy,     // めまい(混乱)
		eRun,		// 移動

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// 球体のモンスターのインスタンス
	static CBeholder* spInstance;

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// 球体のモンスターの状態
	enum class EState
	{
		eIdle,		// 待機
		eIdle2,		// 待機2
		eAttack,	// 攻撃
		eAttack2,	// 攻撃2
		eAttack3,	// 攻撃3
		eAttack4,	// 攻撃4
		eAttackWait,// 攻撃終了待ち
		eHit,       // ヒット
		eDie,       // 死ぬ時
		eDizzy,     // めまい(混乱)
		eRun,       // 移動
	};
	EState mState;	// 球体のモンスターの状態

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか

	CColliderLine* mpColliderLine;

	CColliderSphere* mpColliderSphere;   // キャラクター押し戻しコライダー (頭)
	CColliderSphere* mpColliderSphere2;  // キャラクター押し戻しコライダー (左上の触手)
	CColliderSphere* mpColliderSphere3;  // ダメージを受けるコライダー(右上の触手)
	CColliderSphere* mpColliderSphere4;  // ダメージを受けるコライダー(左下の触手)
	CColliderSphere* mpColliderSphere5;  // ダメージを受けるコライダー(右下の触手)
	CColliderSphere* mpColliderSphere6;  // ダメージを受けるコライダー(真ん中上の触手)
	CColliderSphere* mpColliderSphere7;  // ダメージを受けるコライダー(真ん中下の触手)

	CColliderSphere* mpDamageCol;   // ダメージを受けるコライダー(体)
	CColliderSphere* mpDamageCol2;  // ダメージを受けるコライダー(左上の触手)
	CColliderSphere* mpDamageCol3;  // ダメージを受けるコライダー(右上の触手)
	CColliderSphere* mpDamageCol4;  // ダメージを受けるコライダー(左下の触手)
	CColliderSphere* mpDamageCol5;  // ダメージを受けるコライダー(右下の触手)
	CColliderSphere* mpDamageCol6;  // ダメージを受けるコライダー(真ん中上の触手)
	CColliderSphere* mpDamageCol7;  // ダメージを受けるコライダー(真ん中下の触手)

	CColliderSphere* mpAttackCol;   // ダメージを与えるコライダー(体)
	CColliderSphere* mpAttackCol2;  // ダメージを与えるコライダー(左上の触手)
	CColliderSphere* mpAttackCol3;  // ダメージを与えるコライダー(右上の触手)
	CColliderSphere* mpAttackCol4;  // ダメージを与えるコライダー(左下の触手)
	CColliderSphere* mpAttackCol5;  // ダメージを与えるコライダー(右下の触手)
	CColliderSphere* mpAttackCol6;  // ダメージを与えるコライダー(真ん中上の触手)
	CColliderSphere* mpAttackCol7;  // ダメージを与えるコライダー(真ん中下の触手)
	CTransform* mpRideObject;
};