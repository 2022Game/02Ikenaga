#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

class CWaveEffect;

/*
 エイ3
 プレイヤーの推定レベル(30)
 エネミークラスを継承
*/
class CRay3 : public CEnemy
{
public:
	//インスタンスのポインタの取得
	static CRay3* Instance();

	// コンストラクタ
	CRay3();
	~CRay3();

	// 待機状態
	void UpdateIdle();

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

	// エイ3のインスタンス
	static CRay3* spInstance;

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// エイ3の状態
	enum class EState
	{
		eIdle,		// 待機
		eAttack,	// 攻撃
		eAttackWait,// 攻撃終了待ち
		eHit,       // ヒット
		eDie,       // 死ぬ時
		eRun,		// 移動
	};
	EState mState;	// エイ3の状態

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphere;  // キャラクター押し戻しコライダー
	CColliderSphere* mpDamageCol;  // ダメージを受けるコライダー
	CColliderSphere* mpAttackCol;  // ダメージを与えるコライダー
	CTransform* mpRideObject;

	CWaveEffect* mpWave;
};