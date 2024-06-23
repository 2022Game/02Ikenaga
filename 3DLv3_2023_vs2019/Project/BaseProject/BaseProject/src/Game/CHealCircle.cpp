#include "CHealCircle.h"
#include "CCollisionManager.h"
#include "CPlayer.h"
#include "CStarAura.h"
#include "Maths.h"

CHealCircle* CHealCircle::spInstance = nullptr;

// �R���X�g���N
CHealCircle::CHealCircle(float angle, float dist)
	: mCount(0)
	, mElapsedHealTime(0.0f)
	, mAngle(angle)
	, mDistance(dist)
	, mBaseScale(1.0f)
{
	spInstance = this;
	mpHealCircle = CResourceManager::Get<CModel>("HealCircle");
	mpHealCircle->SetupEffectSettings();
}

// �f�X�g���N�^
CHealCircle::~CHealCircle()
{
}

// �C���X�^���X
CHealCircle* CHealCircle::Instance()
{
	return spInstance;
}

// �������ݒ�
void CHealCircle::SetOwner(CCharaBase* owner)
{
	CWeapon::SetOwner(owner);
	mBaseScale = owner->Scale().X();
}

// �񕜃T�[�N���J�n
void CHealCircle::StartCircle()
{
	mCount += 1;
}

// �X�V
void CHealCircle::Update()
{
	// ������̃x�[�X�X�P�[���l����A
	// ���݂̊g�嗦�����߂āA�񕜃I�[���ɂ����f
	float scale = mOwner->Scale().X() / mBaseScale;
	Scale(scale, scale, scale);

	// ��]����񕜃I�[���̒��S���W
	CVector center = mOwner->Position();
	CVector pos = CVector::zero;
	pos.Y(0.0f);
	Position(center + pos * scale);
	Rotate(0.0f, 0.3f, 0.0f);

	
	if (mCount >= 1)
	{	
		mElapsedHealTime += Time::DeltaTime();
		if (mElapsedHealTime >= 0.0f)
		{
			CTask::SetShow(true);
			CStarAura* star = new CStarAura
			(
				center,
				VectorY(),
				10.0f,
				20.0f
			);
			star->SetColor(CColor(0.0f, 1.0f, 0.0f));
			center.X(center.X() + Math::Rand(-40.0f, 40.0f));
			center.Y(center.Y() + Math::Rand(-30.0f, 30.0f));
			center.Z(center.Z() + Math::Rand(-40.0f, 40.0f));
			star->Position(center);
		}
		if (mElapsedHealTime >= 1.5f)
		{
			CTask::SetShow(false);
			mElapsedHealTime = 0.0f;
			mCount = 0;
		}
	}
}

// �`��
void CHealCircle::Render()
{
	if (mpAttachMtx == nullptr)
	{
		mpHealCircle->SetColor(mColor);
		mpHealCircle->Render(Matrix());
	}
	else
	{
		CMatrix sm;
		sm.Scale(100.0f, 100.0f, 100.0f);
		mpHealCircle->Render(sm * (*mpAttachMtx));
	}
}