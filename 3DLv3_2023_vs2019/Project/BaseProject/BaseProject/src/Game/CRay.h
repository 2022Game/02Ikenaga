#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"

class CWaveEffect;

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
	~CRay();

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
		eIdle,		// 待機
		eIdle2,		// 待機2
		eAttack,	// 攻撃
		eAttackWait,// 攻撃終了待ち
		eHit,       // ヒット
		eDie,       // 死ぬ時
		eRun,		// 移動
	};
	EState mState;	// エイの状態

	int mStateAttackStep;  // State内の攻撃でのステップ処理

	// 状態を切り替え
	void ChangeState(EState state);

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか

	CColliderLine* mpColliderLine;          // キャラクターの軸

	CColliderSphere* mpColliderSphereHead;  // キャラクターの押し戻しコライダー(頭)
	CColliderSphere* mpColliderSphereBody;  // キャラクターの押し戻しコライダー(体)

	CColliderCapsule* mpDamageColSpine;     // ダメージを受けるコライダー(脊椎)
	CColliderSphere* mpDamageColFinLeft;    // ダメージを受けるコライダー(ヒレの左)
	CColliderSphere* mpDamageColFinRight;   // ダメージを受けるコライダー(ヒレの右)

	CColliderSphere* mpAttackColHead;       // 攻撃コライダー(頭)
	CTransform* mpRideObject;

	CWaveEffect* mpWave;  // 気泡
};