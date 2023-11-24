#include "CObjectBase.h"
#include "CCollider.h"

// コンストラクタ
CObjectBase::CObjectBase(ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CTask(prio, sortOrder, pause)
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