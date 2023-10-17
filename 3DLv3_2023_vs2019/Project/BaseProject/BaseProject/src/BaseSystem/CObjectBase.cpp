#include "CObjectBase.h"
#include "CCollider.h"

// コンストラクタ
CObjectBase::CObjectBase(ETag tag, ETaskPriority prio)
	: CTask(prio)
	, mTag(tag)
{
}

// デストラクタ
CObjectBase::~CObjectBase()
{
}

// オブジェクトタグを取得
ETag CObjectBase::Tag() const
{
	return mTag;
}

// 衝突処理
void CObjectBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}