#ifndef CRICH_H
#define CRICH_H
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CModel.h"
class CCane;

/*
リッチクラス
キャラクタクラスを継承
*/
class CRich : public CXCharacter
{
public:
	//インスタンスのポインタの取得
	static CRich* Instance();

	// コンストラクタ
	CRich();
	// デストラクタ
	~CRich();

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

	// ヒット
	void UpdateHit();

	// 更新処理
	void Update();

	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

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

	// 描画
	void Render();
private:
	int mAttackTime;  // 攻撃時間の間隔

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,	   // Tポーズ
		eIdle,	   // 待機
		eAttack,   // 攻撃
		eAttack2,  // 攻撃2
		eHit,      // ヒット
		eWalk,	   // 歩行

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// リッチのインスタンス
	static CRich* spInstance;

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

	// リッチの状態
	enum class EState
	{
		eIdle,		  // 待機
		eIdle2,		  // 待機2
		eAttack,	  // 攻撃
		eAttack2,	  // 攻撃2
		eAttackWait,  // 攻撃終了待ち
		eHit,         // ヒット	
	};
	EState mState;	  // エネミーの状態

	// 状態を切り替え
	void ChangeState(EState state);

	bool mIsGrounded; // 接地しているかどうか

	// 線分コライダー
	CColliderLine* mpColliderLine;
	// ダメージを受けるコライダー
	CColliderCapsule* mpDamageColBody;  // 体
	CColliderSphere* mpDamageColArmL;   // 左腕
	CColliderSphere* mpDamageColArmR;   // 右腕

	CTransform* mpRideObject;
	CCane* mpCane;  // 杖
};

#endif
