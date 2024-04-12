#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

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
	~CChest();

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
	// ダメージを与えたオブジェクト
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
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// チェストモンスターの状態
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
		eRun        // 移動
	};
	EState mState;	// チェストモンスターの状態

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか
	bool mIsSpawnedCoinEffect;

	CColliderLine* mpColliderLine;

	CColliderSphere* mpColliderSphereHead;   // キャラクター押し戻しコライダー(頭)
	CColliderSphere* mpColliderSphereBody;   // キャラクター押し戻しコライダー(体)
	CColliderSphere* mpColliderSphereFeet;   // キャラクター押し戻しコライダー(前の左足)
	CColliderSphere* mpColliderSphereFeet2;  // キャラクター押し戻しコライダー(前の右足)
	CColliderSphere* mpColliderSphereFeet3;  // キャラクター押し戻しコライダー(後ろの左足)
	CColliderSphere* mpColliderSphereFeet4;  // キャラクター押し戻しコライダー(後ろの右足)

	CColliderSphere* mpDamageColHead;    // ダメージを受けるコライダー(頭)
	CColliderSphere* mpDamageColBody;    // ダメージを受けるコライダー(体)
	CColliderSphere* mpDamageColFeet;    // ダメージを受けるコライダー(前の左足)
	CColliderSphere* mpDamageColFeet2;   // ダメージを受けるコライダー(前の右足)
	CColliderSphere* mpDamageColFeet3;   // ダメージを受けるコライダー(後ろの左足)
	CColliderSphere* mpDamageColFeet4;   // ダメージを受けるコライダー(後ろの右足)

	CColliderSphere* mpAttackColHead;   // ダメージを与えるコライダー(頭)
	CTransform* mpRideObject;
};