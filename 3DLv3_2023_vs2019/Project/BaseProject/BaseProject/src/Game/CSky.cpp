#include "CSky.h"
#include "CColliderMesh.h"

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
	: CObjectBase(ETag::eSky, ETaskPriority::eBackground,0,ETaskPauseType::eGame)
{
	spInstance = this;
	mElapsedTime = 0.0f;

	mpModel = CResourceManager::Get<CModel>("Sky");

	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);
}

// デストラクタ
CSky::~CSky()
{
	SAFE_DELETE(mpColliderMesh);
}

// コライダーを取得
CCollider* CSky::GetCollider() const
{
	return mpColliderMesh;
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
}

// 描画
void CSky::Render()
{
	mpModel->Render(Matrix());
}