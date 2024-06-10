#include "CRoar.h"
#include "CCharaBase.h"

// コンストラクタ
CRoar::CRoar(CObjectBase* owner, const CVector& pos, const CVector& dir,
	float speed, float dist)
	: mpOwner(owner)
	, mKillMoveDist(dist)
	, mMovedDist(0.0f)
{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
	Rotation(CQuaternion::LookRotation(mMoveSpeed, CVector::up));

	mpModel = CResourceManager::Get<CModel>("Roar");
	mpModel->SetupEffectSettings();
}

// デストラクタ
CRoar::~CRoar()
{
}

// 更新
void CRoar::Update()
{
	CVector move = mMoveSpeed * Time::DeltaTime();
	float dist = move.Length();
	if (mMovedDist + dist >= mKillMoveDist)
	{
		dist = mKillMoveDist - mMovedDist;
		move = move.Normalized() * dist;
	}
	Position(Position() + move);

	mMovedDist += dist;
	if (mMovedDist >= mKillMoveDist)
	{
		Kill();
	}
}

// 描画
void CRoar::Render()
{
	mpModel->SetColor(mColor);
	mpModel->Render(Matrix());
}