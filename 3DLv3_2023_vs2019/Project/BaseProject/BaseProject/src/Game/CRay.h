#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"

class CHit;

/*
 エイ
 プレイヤーの推定レベル(21～30)
 エネミークラスを継承
*/
class CRay : public CEnemy
{
public:
	//インスタンスのポインタの取得
	static CRay* Instance();

	// コンストラクタ
	CRay();

	// デストラクタ
	~CRay();

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

	// ランダムに位置を取得
	CVector GetRandomSpawnPos()override;

	// 描画
	void Render();

private:

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

	float mAttackTime;   // 次の攻撃時間
	float mFlyingTime;   // 飛行時間

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,	  // Tポーズ
		eIdle,	  // 待機
		eAttack,  // 攻撃
		eHit,     // ヒット
		eDie,     // 死ぬ
		eRun,	  // 移動

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// 波動のエフェクトを作成
	void CreateWave();

	// エイのインスタンス
	static CRay* spInstance;

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

	// エイの状態
	enum class EState
	{
		eIdle,		  // 待機
		eIdle2,		  // 待機2
		eAttack,	  // 攻撃
		eAttackWait,  // 攻撃終了待ち
		eHit,         // ヒット
		eDie,         // 死ぬ時
		eRun,		  // 移動
	};
	EState mState;	  // エイの状態

	int mStateAttackStep;  // State内の攻撃でのステップ処理
	int mStateWave;        // State内の波動のステップ処理

	// 状態を切り替え
	void ChangeState(EState state);

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか

	// 線分コライダー
	CColliderLine* mpColLineSide;           // 線分コライダー(横)
	CColliderLine* mpColLineHeight;         // 線分コライダー(縦)

	// キャラクターの押し戻しコライダー
	CColliderSphere* mpColliderSphereHead;  // 頭
	CColliderSphere* mpColliderSphereBody;  // 体

	 // ダメージを受けるコライダー
	CColliderCapsule* mpDamageColSpine;     // 脊椎
	CColliderSphere* mpDamageColFinLeft;    // ヒレの左
	CColliderSphere* mpDamageColFinRight;   // ヒレの右

	// 攻撃コライダー
	CColliderSphere* mpAttackColHead;       //頭

	CTransform* mpRideObject;

	// エフェクト関連
	bool mIsSpawnedWaveEffect;  // 波動エフェクト
	float mElapsedWaveTime;     // 経過時間計測用(波動)
	CHit* mpHitEffect;          // ヒットエフェクト
};