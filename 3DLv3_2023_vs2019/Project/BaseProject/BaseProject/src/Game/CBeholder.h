#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

class CLightningBallEffect;
class CElectricShockEffect;
class CHomingBallEffect;
class CCurrent;
class CHit;
class CDizzyEffect;
class CGameEnemyUI;

/*
 球体のモンスター
 プレイヤーの推定レベル(61～70)
 エネミークラスを継承
*/
class CBeholder : public CEnemy
{
public:
	//インスタンスのポインタの取得
	static CBeholder* Instance();

	// コンストラクタ
	CBeholder();

	// デストラクタ
	~CBeholder();

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

	// 戦う前の待機状態
	void UpdateIdle();
	// 戦う前の待機状態2
	void UpdateIdle2();

	// 攻撃(電気ボール)
	void UpdateAttack();
	// 攻撃2(電流)
	void UpdateAttack2();
	// 攻撃3(電気ボールのホーミング)
	void UpdateAttack3();
	// 攻撃4(回転攻撃＋エフェクト)
	void UpdateAttack4();
	// 攻撃終了待ち
	void UpdateAttackWait();

	//ヒット
	void UpdateHit();
	// 死ぬ時
	void UpdateDie();
	// めまい(混乱)
	void UpdateDizzy();

	float mAttackTime;  // 次の攻撃時間
	float mFlyingTime;  // 飛行時間  

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ
		eIdle,		// 待機
		eIdle2,		// 待機2
		eAttack,	// 攻撃(電気ボール)
		eAttack2,	// 攻撃2(電流)
		eAttack3,	// 攻撃3(電気ボールのホーミング)
		eAttack4,	// 攻撃4(回転攻撃＋エフェクト)
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
		float animSpeed;    // アニメーションの再生速度
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// 球体のモンスターの状態
	enum class EState
	{
		eIdle,		  // 待機
		eIdle2,		  // 待機2
		eAttack,	  // 攻撃(電気ボール)
		eAttack2,	  // 攻撃2(電流)
		eAttack3,	  // 攻撃3(電気ボールのホーミング)
		eAttack4,	  // 攻撃4(回転攻撃＋エフェクト)
		eAttackWait,  // 攻撃終了待ち
		eHit,         // ヒット
		eDie,         // 死ぬ時
		eDizzy,       // めまい(混乱)
		eRun,         // 移動
	};
	EState mState;	  // 球体のモンスターの状態
	int mStateStep;   // State内のステップ処理

	// 状態を切り替え
	void ChangeState(EState state);

	// トルネードエフェクトを生成
	void CreateTornado();

	// 電流エフェクトを生成
	void CreateCurrent();
	// 電流エフェクトにヒットした
	void CollisionCurrent();

	void CreateHit();

	CVector mMoveSpeed;	  // 移動速度
	bool mIsGrounded;	  // 接地しているかどうか

	 // 線分コライダー
	CColliderLine* mpColLineSide;    // 線分コライダー(横)
	CColliderLine* mpColLineHeight;  // 線分コライダー(縦)

	// キャラクター押し戻しコライダー
	CColliderSphere* mpColliderSphereBody;       // 体
	CColliderSphere* mpColliderSphereTentacle;   // 左上の触手
	CColliderSphere* mpColliderSphereTentacle2;  // 右上の触手
	CColliderSphere* mpColliderSphereTentacle3;  // 左下の触手
	CColliderSphere* mpColliderSphereTentacle4;  // 右下の触手
	CColliderSphere* mpColliderSphereTentacle5;  // 真ん中上の触手
	CColliderSphere* mpColliderSphereTentacle6;  // 真ん中下の触手

	 // ダメージを受けるコライダー
	CColliderSphere* mpDamageColBody;            // 体
	CColliderSphere* mpDamageColTentacle;        // 左上の触手
	CColliderSphere* mpDamageColTentacle2;       // 右上の触手
	CColliderSphere* mpDamageColTentacle3;       // 左下の触手
	CColliderSphere* mpDamageColTentacle4;       // 右下の触手
	CColliderSphere* mpDamageColTentacle5;       // 真ん中上の触手
	CColliderSphere* mpDamageColTentacle6;       // 真ん中下の触手

	// ダメージを与えるコライダー
	CColliderSphere* mpAttackColBody;            // 体
	CColliderSphere* mpAttackColTentacle;        // 左上の触手
	CColliderSphere* mpAttackColTentacle2;       // 右上の触手
	CColliderSphere* mpAttackColTentacle3;       // 左下の触手
	CColliderSphere* mpAttackColTentacle4;       // 右下の触手
	CColliderSphere* mpAttackColTentacle5;       // 真ん中上の触手
	CColliderSphere* mpAttackColTentacle6;       // 真ん中下の触手

	CTransform* mpRideObject;

	CLightningBallEffect* mpLightningBall;  // 雷球
	CElectricShockEffect* mpElectricShock;  // 電撃
	CHomingBallEffect* mpHomingBall;  // ホーミングボール

	// 電流エフェクト
	CCurrent* mpCurrent;
	CCurrent* mpCurrent2;
	CCurrent* mpCurrent3;
	CCurrent* mpCurrent4;
	CCurrent* mpCurrent5;
	CCurrent* mpCurrent6;

	// ヒットエフェクト
	CHit* mpHitEffect;
	// めまいのエフェクト
	CDizzyEffect* mpDizzyEffect;
};