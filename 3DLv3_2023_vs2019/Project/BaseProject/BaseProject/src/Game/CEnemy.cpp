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

// �������X���C���̎��S����
void CEnemy::SlimeDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::SlimeSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// �傫���X���C���̎��S����
void CEnemy::SlimeBigDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::SlimeBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// �������}�b�V�����[���̎��S����
void CEnemy::MushroomDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::MushroomSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// �傫���}�b�V�����[���̎��S����
void CEnemy::MushroomBigDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::MushroomBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// �������T�̎��S����
void CEnemy::TurtleDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::TurtleSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// �傫���T�̎��S����
void CEnemy::TurtleBigDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::TurtleBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// ���S����(���x��50�ȉ��̓G�p)
void CEnemy::Death()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::SpawnExp(
		mCharaStatus.exp,
		Position()+CVector(0.0f,2.0f,0.0f)
	);
}

// ���S����(���x��90�ȉ��̓G�p)
void CEnemy::Death2()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::SpawnExp2(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}