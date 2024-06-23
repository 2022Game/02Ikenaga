#include "CHealAuar.h"
#include "CCollisionManager.h"
#include "CPlayer.h"
#include "Maths.h"

// �R���X�g���N
CHealAura::CHealAura(float angle, float dist)
	: mCount(0)
	, mElapsedHealTime(0.0f)
	, mAngle(angle)
	, mDistance(dist)
	, mBaseScale(1.0f)
{
	mpHealAura = CResourceManager::Get<CModel>("Aura");
	mpHealAura->SetupEffectSettings();
}

// �f�X�g���N�^
CHealAura::~CHealAura()
{
}

// �������ݒ�
void CHealAura::SetOwner(CCharaBase* owner)
{
	CWeapon::SetOwner(owner);
	mBaseScale = owner->Scale().X();
}

// �I�[���J�n
void CHealAura::StartAura()
{
	mCount += 1;
}

// �X�V
void CHealAura::Update()
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
void CHealAura::Render()
{
	if (mpAttachMtx == nullptr)
	{
		mpHealAura->SetColor(mColor);
		mpHealAura->Render(Matrix());
	}
	else
	{
		CMatrix sm;
		sm.Scale(100.0f, 100.0f, 100.0f);
		mpHealAura->Render(sm * (*mpAttachMtx));
	}
}