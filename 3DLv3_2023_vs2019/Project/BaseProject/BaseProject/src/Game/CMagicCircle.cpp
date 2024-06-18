#include "CMagicCircle.h"
#include "CCharaBase.h"
#include "Easing.h"

// 魔法陣のスケール値の最大値
#define FLAME_SCALE 1.0f
// 魔法陣のスケール値が最大値になるまでの時間
#define FLAME_SCALE_ANIM_TIME 0.2f

// コンストラクタ
CMagicCircle::CMagicCircle(CObjectBase* owner, const CVector& pos)
	: mpOwner(owner)
	, mElapsedTime(0.0f)
{
	Position(pos);

	mpModel = CResourceManager::Get<CModel>("MagicCircle");
	mpModel->SetupEffectSettings();
}

// デストラクタ
CMagicCircle::~CMagicCircle()
{
}

// 更新
void CMagicCircle::Update()
{
	Rotate(0.0f, 0.5f, 0.0f);
	mElapsedTime += Time::DeltaTime();
	if (mElapsedTime >= 10)
	{
		CTask::Kill();
		//CTask::SetShow(false);
	}
}

// 描画
void CMagicCircle::Render()
{
	mpModel->SetColor(mColor);
	mpModel->SetBlend(EBlend::eAdd);
	mpModel->Render(Matrix());
}