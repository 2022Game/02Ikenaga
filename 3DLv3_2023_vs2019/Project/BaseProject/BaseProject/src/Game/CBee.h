#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

/*
 蜂
 プレイヤーの推定レベル(31～40)
 エネミークラスを継承
*/
class CBee : public CEnemy
{
public:
	//インスタンスのポインタの取得
	static CBee* Instance();

	// コンストラクタ
	CBee();
	~CBee();

	// 待機状態
	void UpdateIdle();
	// 待機状態2
	void UpdateIdle2();

	// 攻撃
	void UpdateAttack();
	// 攻撃終了待ち
	void UpdateAttackWait();

	//ヒット
	void UpdateHit();
	// 死ぬ時
	void UpdateDie();

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
	int mFlyingTime;   // 飛行時間

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ
		eIdle,		// 待機
		eAttack,	// 攻撃
		eHit,       // ヒット
		eDie,       // 死ぬ
		eRun,		// 移動

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// 蜂のインスタンス
	static CBee* spInstance;

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

	// 蜂の状態
	enum class EState
	{
		eIdle,		// 待機
		eIdle2,		// 待機2
		eAttack,	// 攻撃
		eAttackWait,// 攻撃終了待ち
		eHit,       // ヒット
		eDie,       // 死ぬ時
		eRun,       // 移動
	};
	EState mState;	// 蜂の状態
	int mStateAttackStep;  // State内の攻撃でのステップ処理

	// 状態を切り替え
	void ChangeState(EState state);

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか
	bool mIsSpawnedNeedleEffect;

	CColliderLine* mpColliderLine;

	CColliderSphere* mpColliderSphereHead;   // キャラクター押し戻しコライダー(頭)
	CColliderSphere* mpColliderSphereBeak;   // キャラクター押し戻しコライダー(口ばし)
	CColliderSphere* mpColliderSphereBeak2;  // キャラクター押し戻しコライダー(口ばし2)
	CColliderSphere* mpColliderSphereBody;   // キャラクター押し戻しコライダー(体)
	CColliderSphere* mpColliderSphereTail;   // キャラクター押し戻しコライダー(尻尾)
	CColliderSphere* mpColliderSphereTail2;  // キャラクター押し戻しコライダー(尻尾2)
	CColliderSphere* mpColliderSphereTail3;  // キャラクター押し戻しコライダー(尻尾3)
	CColliderSphere* mpColliderSphereTail4;  // キャラクター押し戻しコライダー(尻尾4)
	CColliderSphere* mpColliderSphereTail5;  // キャラクター押し戻しコライダー(尻尾5)

	CColliderSphere* mpDamageColHead;    // ダメージを受けるコライダー(頭)
	CColliderSphere* mpDamageColBeak;    // ダメージを受けるコライダー(口ばし)
	CColliderSphere* mpDamageColBeak2;   // ダメージを受けるコライダー(口ばし2)
	CColliderSphere* mpDamageColBody;    // ダメージを受けるコライダー(体)
	CColliderSphere* mpDamageColTail;    // ダメージを受けるコライダー(尻尾)
	CColliderSphere* mpDamageColTail2;   // ダメージを受けるコライダー(尻尾2)
	CColliderSphere* mpDamageColTail3;   // ダメージを受けるコライダー(尻尾3)
	CColliderSphere* mpDamageColTail4;   // ダメージを受けるコライダー(尻尾4)
	CColliderSphere* mpDamageColTail5;   // ダメージを受けるコライダー(尻尾5)

	CColliderSphere* mpAttackCol;        // ダメージを与えるコライダー(針)

	CTransform* mpRideObject;
};