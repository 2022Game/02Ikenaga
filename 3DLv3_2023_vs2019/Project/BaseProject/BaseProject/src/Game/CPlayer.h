#pragma once
//キャラクタクラスのインクルード
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderSphere.h"
#include "CRideableObject.h"
#include "CImage.h"

#include <algorithm>
/*
プレイヤークラス
キャラクタクラスを継承
*/
class CPlayer : public CXCharacter
{
public:
	bool IsDeath() const;
	bool IsDeath2() const;
	bool IsDeath3() const;
	static int Hp();
	static int Level();
	static int Power();
	//インスタンスのポインタの取得
	static CPlayer* Instance();

	// コンストラクタ
	CPlayer();
	// デストラクタ
	~CPlayer();

	// 待機状態
	void UpdateIdle();
	// 攻撃
	void UpdateAttack();
	// 攻撃2
	void UpdateAttack2();
	// 攻撃終了待ち
	void UpdateAttackWait();
	// ジャンプ開始
	void UpdateJumpStart();
	// ジャンプ中
	void UpdateJump();
	// ジャンプ終了
	void UpdateJumpEnd();
	//攻撃力アップ
	void UpdatePowerUp();
	//攻撃力アップ終了
	void UpdatePowerUpEnd();
	//レベル(HP・攻撃力・大きさ)の更新
	void UpdateLevel();

	// 更新
	void Update();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 描画
	void Render();

private:
	static int  sHp;  //HP
	static int sLevel;  //レベル
	static int sPower;  //攻撃力
	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		//eTPose,		// Tポーズ
		eIdle,		// 待機
		eWalk,		// 歩行
		eAttack,	// 攻撃
		//eAttack2,	// 攻撃2
		eJumpStart,	// ジャンプ開始
		ePowerUp,   //攻撃力アップ
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// プレイヤーのインスタンス
	static CPlayer* spInstance;

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// プレイヤーの状態
	enum class EState
	{
		eIdle,		// 待機
		eAttack,	// 攻撃
		eAttack2,	// 攻撃2
		eAttackWait,// 攻撃終了待ち
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了
		ePowerUp,   //攻撃力アップ
		ePowerUpEnd,   //攻撃力アップ終了
	};
	EState mState;	// プレイヤーの状態

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphere;

	CTransform* mpRideObject;
	CImage* mImage;
	CImage* mImage2;
};
