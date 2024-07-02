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
	bool PortionBlue = false;
	bool PortionGreen = false;
	bool PortionRed = false;

	float rand2 = Math::Rand(0, 3);
	float rand3 = Math::Rand(4, 7);

	if (rand2 == 3)PortionGreen = true;
	if (rand3 == 7) PortionRed = true;

	if (PortionGreen)
	{
		// �񕜃|�[�V����
		CPortionGreen* portiongreen = new CPortionGreen();
		portiongreen->Position(pos);
		portiongreen->Scale(200.0f, 200.0f, 200.0f);
	}
	else if (PortionRed)
	{
		// �U���̓A�b�v�|�[�V����
		CPortionRed* portionred = new CPortionRed();
		portionred->Position(pos);
		portionred->Scale(200.0f, 200.0f, 200.0f);
	}
	else
	{
		// �h��̓A�b�v�|�[�V����
		CPortionBlue* portionblue = new CPortionBlue();
		portionblue->Position(pos);
		portionblue->Scale(200.0f, 200.0f, 200.0f);
	}
}