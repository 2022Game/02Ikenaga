#include "CNightSky.h"
#include "CSky.h"

// コンストラク
CNightSky::CNightSky()
	: CObjectBase(ETag::eSky, ETaskPriority::eBackground)
	, mElapsedTime(0.0f)
{
	mpModel = CResourceManager::Get<CModel>("NightSky");
}

// デストラクタ
CNightSky::~CNightSky()
{
}

// 更新
void CNightSky::Update()
{
	Rotate(0.0f, 0.01f, 0.0f);

	CSky* sky = sky->Instance();
	mElapsedTime = sky->mElapsedTime;
	if (mElapsedTime >= 50)
	{
		CTask::SetShow(true);
	}
	else
	{
		CTask::SetShow(false);
	}
	if (mElapsedTime >= 70)
	{
		CTask::SetShow(false);
	}
}

// 描画
void CNightSky::Render()
{
	mpModel->Render(Matrix());
}