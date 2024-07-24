#pragma once
#include "CColliderLine.h"
#include "CColliderSphere.h"
#include "CEnemy.h"
#include "CSound.h"

class CHit;

/*
 レッドスライムのクラス
 プレイヤーの推定レベル(1～5)
 エネミークラスを継承
*/
class CSlime : public CEnemy
{
public:

	bool IsDeath() const;
	//インスタンスのポインタの取得
	static CSlime* Instance();

	// コンストラクタ
	CSlime();
	// デストラクタ
	~CSlime();

	// 更新処理
	void Update();

	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	virtual void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 攻撃開始
	void AttackStart() override;
	// 攻撃終了
	void AttackEnd() override;

	// 1レベルアップ
	void LevelUp();
	// レベルの変更
	void ChangeLevel(int level);

	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	/// <param name="damage">受けるダメージ</param>
	//ダメージを与えたオブジェクト
	virtual void TakeDamage(int damage, CObjectBase*causedObj);

	// 死亡処理
	void Death() override;

	// ランダムに位置を取得
	CVector GetRandomSpawnPos()override;

	// 描画
	void Render();

private:

	// 待機状態
	void UpdateIdle();
	// 待機2状態
	void UpdateIdle2();
	// プレイヤーの攻撃がヒットした時の待機状態
	void UpdateIdle3();
	// 待機2の終了待ち
	void UpdateIdleWait();

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

	int mAttackTime;   // 攻撃時間の間隔

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ
		eIdle,		// 待機
		eIdle2,		// 待機2
		eIdle3,		// 待機3
		eIdle4,		// 待機4
		eAttack,	// 攻撃
		eAttack2,	// 攻撃2
		eHit,       // ヒット
		eDie,       // 死ぬ時
		eDizzy,     // めまい(混乱)
		eRun,		// 移動

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// インスタンス
	static CSlime* spInstance;

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

	// レッドスライムの状態
	enum class EState
	{
		eIdle,		// 待機
		eIdle2,     // 待機2
		eIdle3,     // 待機3
		eIdleWait,  // 待機終了待ち
		eAttack,	// 攻撃
		eAttack2,	// 攻撃2
		eAttackWait,// 攻撃終了待ち
		eHit,       // ヒット
		eDie,       // 死ぬ時
		eDizzy,     // めまい(混乱)
		eRun,       // 移動
	};
	EState mState;	// レッドスライムの状態
	int mStateStep; // State内のステップ処理

	// 状態を切り替え
	void ChangeState(EState state);

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか

	CColliderLine* mpColliderLine;          // キャラクターの線分コライダー
	CColliderSphere* mpColliderSphereBody;  // キャラクターの押し戻しコライダー(体)
	CColliderSphere* mpDamageColBody;       // ダメージを受けるコライダー(体)
	CColliderSphere* mpAttackColBody;       // ダメージを与えるコライダー(体)
	CTransform* mpRideObject;

	CHit* mpHitEffect;        // ヒットエフェクト 

	// サウンド関連
	CSound* mpSlimeAttackSE;  // 攻撃の音
	CSound* mpSlimeDizzySE;   // 混乱の音
	CSound* mpSlimeHitSE;     // ヒットの音
	CSound* mpSlimeDieSE;     // 死亡の音

	bool mIsSlimeAttackSE;
	bool mIsSlimeDizzySE;
	bool mIsSlimeHitSE;
	bool mIsSlimeDieSE;
};
