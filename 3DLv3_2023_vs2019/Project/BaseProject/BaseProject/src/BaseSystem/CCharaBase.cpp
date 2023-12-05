#include "CCharaBase.h"

CCharaBase::CCharaBase(ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(tag, prio, sortOrder, pause)
{
	mCharaMaxStatus.Reset();
	mCharaStatus.Reset();
}

CCharaBase::~CCharaBase()
{
}

// 被ダメージ処理
void CCharaBase::TakeDamage(int damage, CObjectBase* causedObj)
{
}

// 最大ステータスを取得
const CharaStatus& CCharaBase::MaxStatus() const
{
	return mCharaMaxStatus;
}

// 現在のステータスを取得
const CharaStatus& CCharaBase::Status() const
{
	return mCharaStatus;
}
