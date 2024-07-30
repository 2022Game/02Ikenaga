#include "CSky.h"
#include "CColliderMesh.h"

// �C���X�^���X
CSky* CSky::spInstance = nullptr;

float CSky::mElapsedTime;

// �C���X�^���X
CSky* CSky::Instance()
{
	return spInstance;
}

// �R���X�g���N
CSky::CSky()
	: CObjectBase(ETag::eSky, ETaskPriority::eBackground,0,ETaskPauseType::eGame)
{
	spInstance = this;
	mElapsedTime = 0.0f;

	mpModel = CResourceManager::Get<CModel>("Sky");

	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);
}

// �f�X�g���N�^
CSky::~CSky()
{
	SAFE_DELETE(mpColliderMesh);
}

// �R���C�_�[���擾
CCollider* CSky::GetCollider() const
{
	return mpColliderMesh;
}

// �X�V
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

// �`��
void CSky::Render()
{
	mpModel->Render(Matrix());
}