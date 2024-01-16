#include "CEnemy.h"
#include "CExpManager.h"

// �R���X�g���N�^
CEnemy::CEnemy()
	:CXCharacter(ETag::eEnemy,ETaskPriority::eEnemy)
{
	// HP�Q�[�W�쐬
	mpHpGauge = new CHpGauge(true);
	mpHpGauge->SetCenterRatio(CVector2(0.5, 0.0f));
}



// �f�X�g���N�^
CEnemy::~CEnemy()
{
	// HP�Q�[�W���폜
	mpHpGauge->Kill();
}

// ���S����
void CEnemy::Death()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::SpawnExp(
		mCharaStatus.exp,
		Position()+CVector(0.0f,2.0f,0.0f)
	);
}