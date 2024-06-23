#include "CShieldRotate.h"
#include "CCollisionManager.h"
#include "CPlayer.h"
#include "Maths.h"

// �R���X�g���N
CShieldRotate::CShieldRotate(float angle,float dist)
	: mStateStep(0)
	, mDefenseUp(false)
	, mElapsedDefenseUpTime(0.0f)
	, mAngle(angle)
	, mDistance(dist)
	, mBaseScale(1.0f)
{
	mpShieldRotate = CResourceManager::Get<CModel>("ShieldRotate");
	mpShieldRotate->SetColor(CColor(0.15f, 0.75f, 0.75f));
	mpShieldRotate->SetBlend(EBlend::eAdd);
}

// �f�X�g���N�^
CShieldRotate::~CShieldRotate()
{
}

// �������ݒ�
void CShieldRotate::SetOwner(CCharaBase* owner)
{
	CWeapon::SetOwner(owner);
	mBaseScale = owner->Scale().X();
}

// �X�V
void CShieldRotate::Update()
{
	mDefenseUp = mOwner->IsDefenseUp();
	mElapsedDefenseUpTime = mOwner->GetElapsedDefenseUpTime();

	// ������̃x�[�X�X�P�[���l����A
	// ���݂̊g�嗦�����߂āA���ɂ����f
	float scale = mOwner->Scale().X() / mBaseScale;
	Scale(scale, scale, scale);

	// ��]���鏂�̒��S���W
	CVector center = mOwner->Position();
	// ���S���W���猻�݂̉�]�ō��W�����߂�(�~�^��)
	CVector pos = CVector::zero;
	pos.X(cosf(Math::DegreeToRadian(mAngle)) * mDistance);
	pos.Z(sinf(Math::DegreeToRadian(mAngle)) * mDistance);
	pos.Y(5.0f);
	Position(center + pos * scale);

	// ���݈ʒu���璆�S���W�܂ł̃x�N�g�������߂āA
	// ������̕����֌������
	CVector dif = center - Position();
	dif.Y(0.0f);
	dif.Normalize();
	Rotation(CQuaternion::LookRotation(dif));

	// ���t���[����]�p�x�����Z���āA���S���W�𒆐S�ɉ�]������
	mAngle += 90.0f * Time::DeltaTime();
	if (mAngle >= 360.0f)mAngle -= 360.0f;

	if (mDefenseUp == true)
	{
		switch (mStateStep)
		{
		case 0:
			if (mElapsedDefenseUpTime <= 15.0f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 1:
			if (mElapsedDefenseUpTime >= 15.5f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 2:
			if (mElapsedDefenseUpTime >= 16.0f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 3:
			if (mElapsedDefenseUpTime >= 16.5f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 4:
			if (mElapsedDefenseUpTime >= 17.0f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 5:
			if (mElapsedDefenseUpTime >= 17.5f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 6:
			if (mElapsedDefenseUpTime >= 18.0f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 7:
			if (mElapsedDefenseUpTime >= 18.2f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 8:
			if (mElapsedDefenseUpTime >= 18.4f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 9:
			if (mElapsedDefenseUpTime >= 18.6f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 10:
			if (mElapsedDefenseUpTime >= 18.8f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 11:
			if (mElapsedDefenseUpTime >= 19.0f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 12:
			if (mElapsedDefenseUpTime >= 19.2f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 13:
			if (mElapsedDefenseUpTime >= 19.4f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 14:
			if (mElapsedDefenseUpTime >= 19.6f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 15:
			if (mElapsedDefenseUpTime >= 19.8f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 16:
			if (mElapsedDefenseUpTime >= 19.9f)
			{
				CTask::SetShow(true);
				mStateStep = 0;
			}
			break;
		}
	}
	else
	{
		CTask::SetShow(false);
	}
}

// �`��
void CShieldRotate::Render()
{
	if (mpAttachMtx == nullptr)
	{
		mpShieldRotate->Render(Matrix());
		mpShieldRotate->SetColor(mColor);
	}
	else
	{
		CMatrix sm;
		sm.Scale(100.0f, 100.0f, 100.0f);
		mpShieldRotate->Render(sm * (*mpAttachMtx));
	}
}