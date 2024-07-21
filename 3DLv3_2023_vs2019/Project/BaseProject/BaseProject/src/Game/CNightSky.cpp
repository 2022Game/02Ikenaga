#include "CNightSky.h"
#include "CSky.h"

// �R���X�g���N
CNightSky::CNightSky()
	: CObjectBase(ETag::eSky, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mElapsedTime(0.0f)
{
	mpModel = CResourceManager::Get<CModel>("NightSky");
}

// �f�X�g���N�^
CNightSky::~CNightSky()
{
}

// �X�V
void CNightSky::Update()
{
	Rotate(0.0f, 0.01f, 0.0f);

	CSky* sky = sky->Instance();
	mElapsedTime = sky->mElapsedTime;
	if (mElapsedTime >= 50)
	{
		CTask::SetShow(true);
	}
	else
	{
		CTask::SetShow(false);
	}
	if (mElapsedTime >= 70)
	{
		CTask::SetShow(false);
	}
}

// �`��
void CNightSky::Render()
{
	mpModel->Render(Matrix());
}