#ifndef CENEMY_H
#define CENEMY_H
#include "CXCharacter.h"
#include "CColliderLine.h"

#include "CModel.h"

/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemy : public CXCharacter
{
public:
	//インスタンスのポインタの取得
	static CEnemy* Instance();

	// コンストラクタ
	CEnemy();
	~CEnemy();

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
	// エネミーのインスタンス
	static CEnemy* spInstance;

	bool mIsGrounded;	// 接地しているかどうか

	CColliderLine* mpColliderLine;
	CTransform* mpRideObject;
};

#endif
