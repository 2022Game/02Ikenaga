#pragma once
#include "CPortionBlue.h"
#include "CPortionGreen.h"
#include "CPortionRed.h"

class CPortionManager : public CTask
{
public:

	// �����_���Ń|�[�V��������
	static void  RandomPortion(const CVector& pos);

private:
	// �R���X�g���N
	CPortionManager();
	// �f�X�g���N�^
	~CPortionManager();
};