#include "CTask.h"

CTask::CTask()
	:mpNext(nullptr)
	,mpPrev(nullptr)
	,mPriority(0)
	,mEnabled(true)
	,mIsHidden(false)
{
}

void CTask::SetHidden(bool isHidden)
{
	mIsHidden = isHidden;
}

bool CTask::IsHidden()const
{
	return mIsHidden;
}