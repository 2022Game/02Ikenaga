#pragma once
#include "CTask.h"
#include "CTransform.h"
#include "ObjectTag.h"

class CCollider;

/// <summary>
/// 3D空間に配置するオブジェクトのベースクラス
/// </summary>
class CObjectBase : public CTask, public CTransform
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="tag">オブジェクトのタグ</param>
	/// <param name="prio">タスクの優先度</param>
	CObjectBase(ETag tag = ETag::eNone, ETaskPriority prio = ETaskPriority::eDefault);

	// デストラクタ
	virtual ~CObjectBase();

	// オブジェクトタグを取得
	ETag Tag() const;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	virtual void Collision(CCollider* self, CCollider* other, const CVector& adjust);

private:
	ETag mTag;	//オブジェクト識別用のタグ
};
