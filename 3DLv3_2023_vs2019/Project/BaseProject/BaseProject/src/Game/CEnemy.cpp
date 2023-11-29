#include "CEnemy.h"
#include "CExpManager.h"

// �R���X�g���N�^
CEnemy::CEnemy()
	:CXCharacter(ETag::eEnemy,ETaskPriority::eEnemy)
{

}



// �f�X�g���N�^
CEnemy::~CEnemy()
{

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