#include "CEnemy.h"
#include "CExpManager.h"
#include "CPortionManager.h"
#include "CGameEnemyUI.h"
#include "CEnemyManager.h"
#include "Maths.h"

#define SPAWN_RANDOM_RANGE_X 100.0f
#define SPAWN_RANDOM_RANGE_Z 1000.0f

// �R���X�g���N�^
CEnemy::CEnemy()
	:CXCharacter(ETag::eEnemy,ETaskPriority::eEnemy)
	, mpSummoner(nullptr)
{
	CEnemyManager::Add(this);

	mpGameUI = new CGameEnemyUI();
}

// �f�X�g���N�^
CEnemy::~CEnemy()
{
	CEnemyManager::Remove(this);

	mpGameUI->Kill();

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

CVector CEnemy::GetRandomSpawnPos()
{
	CVector pos = CVector::zero;
	pos.X(Math::Rand(-SPAWN_RANDOM_RANGE_X, SPAWN_RANDOM_RANGE_X));
	pos.Z(Math::Rand(-SPAWN_RANDOM_RANGE_Z, SPAWN_RANDOM_RANGE_Z));
	return pos;
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

// �X�V
void CEnemy::Update()
{
	// HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpGameUI->SetHp(mCharaStatus.hp);
	if (mType == EEnemyType::eDragon)
	{
		mpGameUI->SetScale(1.25f);
	}
	else
	{
		mpGameUI->SetScale(0.5f);
	}
	
	// HP�Q�[�W
	CHpGauge* hpGauge = mpGameUI->GetHpGauge();
	CVector gaugePos = Position() + mpGameUI->GetUIoffSetPos();
	hpGauge->SetWorldPos(gaugePos);
	
	// Lv
	CLevelUI* LvUI = mpGameUI->GetLv();
	CVector lvPos = Position() + mpGameUI->GetUIoffSetPos();
	LvUI->SetWorldPos(lvPos);

	// Level(����)
	CEnemyLevelUI* LevelUI = mpGameUI->GetLevel();
	CVector levelPos = Position() + mpGameUI->GetUIoffSetPos();
	LevelUI->SetWorldPos(levelPos);
	
	// ���O
	CEnemyNameUI* NameUI = mpGameUI->GetName();
	CVector namePos = Position() + mpGameUI->GetUIoffSetPos();
	NameUI->SetWorldPos(namePos);
}
