#include "CSky.h"

// インスタンス
CSky* CSky::spInstance = nullptr;

float CSky::mElapsedTime;

// インスタンス
CSky* CSky::Instance()
{
	return spInstance;
}

// コンストラク
CSky::CSky()
	: CObjectBase(ETag::eSky, ETaskPriority::eBackground)
{
	spInstance = this;
	mElapsedTime = 0.0f;

	mpModel = CResourceManager::Get<CModel>("Sky");
}

// デストラクタ
CSky::~CSky()
{
}

// 更新
void CSky::Update()
{
	mElapsedTime += Time::DeltaTime();
	Rotate(0.0f, 0.01f, 0.0f);

	if (mElapsedTime >= 50)
	{
		CTask::SetShow(false);
	}
	else
	{
		CTask::SetShow(true);
	}
	if (mElapsedTime >= 70)
	{
		mElapsedTime = 0.0f;
	}
	CDebugPrint::Print("時間 %f\n", mElapsedTime);
}

// 描画
void CSky::Render()
{
	mpModel->Render(Matrix());
}