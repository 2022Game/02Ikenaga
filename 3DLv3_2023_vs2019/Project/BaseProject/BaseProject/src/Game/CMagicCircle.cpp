#include "CMagicCircle.h"
#include "CCharaBase.h"
#include "Easing.h"

// ���@�w�̃X�P�[���l�̍ő�l
#define FLAME_SCALE 1.0f
// ���@�w�̃X�P�[���l���ő�l�ɂȂ�܂ł̎���
#define FLAME_SCALE_ANIM_TIME 0.2f

// �R���X�g���N�^
CMagicCircle::CMagicCircle(CObjectBase* owner, const CVector& pos)
	: mpOwner(owner)
	, mElapsedTime(0.0f)
{
	Position(pos);

	mpModel = CResourceManager::Get<CModel>("MagicCircle");
	mpModel->SetupEffectSettings();
}

// �f�X�g���N�^
CMagicCircle::~CMagicCircle()
{
}

// �X�V
void CMagicCircle::Update()
{
	Rotate(0.0f, 0.5f, 0.0f);
	mElapsedTime += Time::DeltaTime();
	if (mElapsedTime >= 8)
	{
		CTask::Kill();
	}
}

// �`��
void CMagicCircle::Render()
{
	mpModel->SetColor(mColor);
	mpModel->SetBlend(EBlend::eAdd);
	mpModel->Render(Matrix());
}