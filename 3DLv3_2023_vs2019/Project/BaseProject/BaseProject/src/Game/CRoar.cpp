#include "CRoar.h"
#include "CCharaBase.h"
#include "Easing.h"

// 雄叫びのスケール値の最大値
#define FLAME_SCALE 90.0f
// 雄叫びのスケール値が最大値になるまでの時間
#define FLAME_SCALE_ANIM_TIME 30.0f

// コンストラクタ
CRoar::CRoar(CObjectBase* owner, const CVector& pos, const CVector& dir,
	float speed, float dist)
	: mpOwner(owner)
	, mKillMoveDist(dist)
	, mMovedDist(0.0f)
	, mElapsedTime(0.0f)
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

	// スケール変更時間を経過していない
	if (mElapsedTime < FLAME_SCALE_ANIM_TIME)
	{
		// 経過時間に合わせて、徐々に雄叫びを大きくする
		float per = mElapsedTime / FLAME_SCALE_ANIM_TIME;
		if (per < 1.0f)
		{
			float scale = Easing::QuadOut(per, 1.0f, 1.0f, 1.0f);
			Scale(CVector::one * scale * FLAME_SCALE);
		}
		else
		{
			Scale(CVector::one * FLAME_SCALE);
		}

		mElapsedTime += Time::DeltaTime();
	}
	// 移動時間が経過したら、削除する
	else
	{
		Scale(CVector::one * FLAME_SCALE);
	}

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