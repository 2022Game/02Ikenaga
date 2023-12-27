#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

/*
 マッシュルームクラス
キャラクタクラスを継承
*/
class CMushroom : public CEnemy
{
public:
	//インスタンスのポインタの取得
	static CMushroom* Instance();

	// コンストラクタ
	CMushroom();
	~CMushroom();

	// 戦う前の待機状態
	void UpdateIdle();
	// 戦う前の待機状態2
	void UpdateIdle2();
	// 待機状態3
	void UpdateIdle3();

	// 攻撃
	void UpdateAttack();
	// 攻撃終了待ち
	void UpdateAttackWait();

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

	// 死亡処理
	void Death() override;

private:
	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ
		eIdle,		// 戦う前の待機
		eIdle2,		// 戦う前の待機2
		eIdle3,     // 待機状態3
		eIdle4,     // 待機状態4
		eAttack,	// 攻撃
		eWalk,		// 歩行
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// マッシュルームのインスタンス
	static CMushroom* spInstance;

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// マッシュルームの状態
	enum class EState
	{
		eIdle,		// 戦う前の待機
		eIdle2,		// 戦う前の待機2
		eIdle3,     // 待機状態3
		eAttack,	// 攻撃
		eAttackWait,// 攻撃終了待ち
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了
	};
	EState mState;	// マッシュルームの状態

	bool mIsGrounded;	// 接地しているかどうか

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphere;
	CColliderSphere* mpDamageCol;  // ダメージを受けるコライダー
	CColliderSphere* mpAttackCol;  // ダメージを与えるコライダー
	CTransform* mpRideObject;
};