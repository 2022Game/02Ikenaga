#include "CPortionManager.h"
#include "Maths.h"

// �R���X�g���N
CPortionManager::CPortionManager()
{
}

// �f�X�g���N�^
CPortionManager::~CPortionManager()
{
}

// �����_���Ń|�[�V��������
void CPortionManager::RandomPortion(const CVector& pos)
{
	float rand = Math::Rand(0,100);
	if (rand <= 30)
	{
		// �U���̓A�b�v�|�[�V����
		CPortionRed* portionred = new CPortionRed();
		portionred->Position(pos);
		portionred->Scale(200.0f, 200.0f, 200.0f);
	}
	else if (rand <= 60)
	{
		// �h��̓A�b�v�|�[�V����
		CPortionBlue* portionblue = new CPortionBlue();
		portionblue->Position(pos);
		portionblue->Scale(200.0f, 200.0f, 200.0f);
	}
	else if(rand <= 90)
	{
		// �񕜃|�[�V����
		CPortionGreen* portiongreen = new CPortionGreen();
		portiongreen->Position(pos);
		portiongreen->Scale(200.0f, 200.0f, 200.0f);
	}
}