#include "CExpManager.h"
#include "Maths.h"

CExpManeger* CExpManeger::spInstance = nullptr;

// �R���X�g���N�^
CExpManeger::CExpManeger()
{
	spInstance = this;
}


//�f�X�g���N�^
CExpManeger::~CExpManeger()
{
	spInstance = nullptr;
}

// �o���l���쐬
void CExpManeger::SpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// �J�n�p�x�̓����_���Ɍ��߂�
	float startAngle = Math::Rand(0.0f, M_PI * 2.0f);
	for (int i = 0; i < exp; i++)
	{
		// ���Ԋu��360�x�o���l���o��������
		float per = (float)i / exp;
		float angle = startAngle + M_PI * 2.0f * per;
		CVector dir = CVector(cosf(angle), 0.0f, sinf(angle));
		CVector epos = pos + dir * 1.0f;
		CExp* e = new CExp();
		e->Setup(epos, dir);
		e->Scale(4.0f, 4.0f, 4.0f);
		e->SetColor(CColor(0.0f, 1.0f, 0.5, 1.0f));
	}

}