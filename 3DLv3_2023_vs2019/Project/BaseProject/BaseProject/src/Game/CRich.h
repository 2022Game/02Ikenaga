#ifndef CRICH_H
#define CRICH_H
#include "CXCharacter.h"
#include "CColliderLine.h"

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
	~CRich();

	// 待機状態
	void UpdateIdle();

	// 更新処理
	void Update();

	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 描画
	void Render();
private:
	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ
		eIdle,		// 待機
		eWalk,		// 歩行
		eAttack,	// 攻撃
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了

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
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// リッチの状態
	enum class EState
	{
		eIdle,		// 待機
		eAttack,	// 攻撃
		eAttackWait,// 攻撃終了待ち
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了
	};
	EState mState;	// エネミーの状態

	bool mIsGrounded;	// 接地しているかどうか

	CColliderLine* mpColliderLine;
	CTransform* mpRideObject;
	CCane* mpCane;  // 杖
};

#endif
