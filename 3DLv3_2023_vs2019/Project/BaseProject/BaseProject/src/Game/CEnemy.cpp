#include "CEnemy.h"
#include "CExpManager.h"
#include "CPortionManager.h"

// �R���X�g���N�^
CEnemy::CEnemy()
	:CXCharacter(ETag::eEnemy,ETaskPriority::eEnemy)
	, mpSummoner(nullptr)
{
	// HP�Q�[�W�쐬
	mpHpGauge = new CHpGauge(true);
	mpHpGauge->SetCenterRatio(CVector2(0.5f, 0.0f));
}

// �f�X�g���N�^
CEnemy::~CEnemy()
{
	// HP�Q�[�W���폜
	mpHpGauge->Kill();

	if (mpSummoner != nullptr)
	{
		mpSummoner->DeathSummonEnemy(this);
	}
}

// �������X���C���̎��S����
void CEnemy::SlimeDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::SlimeSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
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

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
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

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
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

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
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

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
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

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// �������G�C�̎��S����
void CEnemy::RayDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::RaySpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// �傫���G�C�̎��S����
void CEnemy::RayBigDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::RayBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// �������I�̎��S����
void CEnemy::BeeDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::BeeSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// �傫�I�̎��S����
void CEnemy::BeeBigDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::BeeBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// �������T�{�e���̎��S����
void CEnemy::CactusDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::CactusSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// �傫���T�{�e���̎��S����
void CEnemy::CactusBigDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::CactusBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// �������`�F�X�g�����X�^�[�̎��S����
void CEnemy::ChestDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::ChestSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// �傫���`�F�X�g�����X�^�[�̎��S����
void CEnemy::ChestBigDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::ChestBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// ���������̃����X�^�[�̎��S����
void CEnemy::BeholderDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::BeholderSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// �傫�����̃����X�^�[�̎��S����
void CEnemy::BeholderBigDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::BeholderBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// �������{�N�T�[�̎��S����
void CEnemy::BoxerDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::BoxerSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// �傫���{�N�T�[�̎��S����
void CEnemy::BoxerBigDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::BoxerBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// �{�X(�h���S��)�̎��S����
void CEnemy::DragonDeath()
{
	// ���S���Ɍo���l�𐶐�����
	CExpManeger::DragonSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// ���S����
void CEnemy::Death()
{
}

// �G�̎�ނ��擾
EEnemyType CEnemy::GetType() const
{
	return mType;
}

// ���������G�̎��S����
void CEnemy::DeathSummonEnemy(CEnemy* enemy)
{
}

// ���g�����������G��ݒ�
void CEnemy::SetSummoner(CEnemy* summone)
{
	mpSummoner = summone;
}