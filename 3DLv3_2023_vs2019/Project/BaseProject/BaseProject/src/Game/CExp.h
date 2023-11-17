#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CColliderSphere.h"

/*
経験値クラス
オブジェクトベース継承
*/
class CExp : public CObjectBase
{
public:
	CExp();
	~CExp();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	void Update();
	void Render();

private:
	CModel* mpExp;

	CColliderSphere* mpColliderSphere;
};