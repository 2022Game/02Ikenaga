#include "CExpManager.h"
#include "Maths.h"

// �������X���C���p
#define Slime_SPAWN_EXP_COUNT_MIN 1  // ��������o���l�I�[�u�̍ŏ���
#define Slime_SPAWN_EXP_COUNT_MAX 2  // ��������o���l�I�[�u�̍ő吔
// �傫���X���C���p
#define SlimeBig_SPAWN_EXP_COUNT_MIN 3  // ��������o���l�I�[�u�̍ŏ���
#define SlimeBig_SPAWN_EXP_COUNT_MAX 4  // ��������o���l�I�[�u�̍ő吔

// �������}�b�V�����[���p
#define Mushroom_SPAWN_EXP_COUNT_MIN 2  // ��������o���l�I�[�u�̍ŏ���
#define Mushroom_SPAWN_EXP_COUNT_MAX 3  // ��������o���l�I�[�u�̍ő吔
// �傫���}�b�V�����[���p
#define MushroomBig_SPAWN_EXP_COUNT_MIN 4  // ��������o���l�I�[�u�̍ŏ���
#define MushroomBig_SPAWN_EXP_COUNT_MAX 5  // ��������o���l�I�[�u�̍ő吔

// ���x��50�ȉ��̓G�p
#define Level50_SPAWN_EXP_COUNT_MIN 2  // ��������o���l�I�[�u�̍ŏ���
#define Level50_SPAWN_EXP_COUNT_MAX 3  // ��������o���l�I�[�u�̍ő吔

// ���x��90�ȉ��̓G�p
#define Level90_SPAWN_EXP_COUNT_MIN 5  // ��������o���l�I�[�u�̍ŏ���
#define Level90_SPAWN_EXP_COUNT_MAX 7  // ��������o���l�I�[�u�̍ő吔

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

// �o���l���쐬(�������X���C���p)
void CExpManeger::SlimeSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// �o���l�I�[�u�̐�������ݒ�
	int spawnCount = exp;
	// ���������ŏ������傫���ꍇ�́A
	if (spawnCount > Slime_SPAWN_EXP_COUNT_MIN)
	{
		// �����ł���ő吔���擾
		int maxCount = std::min(spawnCount, Slime_SPAWN_EXP_COUNT_MAX);
		// ���������ŏ����`�ő吔�̃����_���Ŏ擾
		spawnCount = Math::Rand(Slime_SPAWN_EXP_COUNT_MIN, maxCount);
	}

	// �J�n�p�x�̓����_���Ɍ��߂�
	float startAngle = Math::Rand(0.0f, M_PI * 2.0f);
	// ���Z����o���l�̃x�[�X�l
	int baseExp = exp / spawnCount;
	// ����؂�Ȃ��]��̌o���l
	int remainExp = exp % spawnCount;
	for (int i = 0; i < spawnCount; i++)
	{
		// �]��̌o���l���c���Ă���ꍇ�́A���Z����o���l�̃x�[�X�l+1����
		CExp* e = new CExp(baseExp + (i < remainExp ? 1 : 0));

		// ���Ԋu��360�x�o���l���o��������
		float per = (float)i / spawnCount;
		float angle = startAngle + M_PI * 2.0f * per;
		CVector dir = CVector(cosf(angle), 0.0f, sinf(angle));
		CVector epos = pos + dir * 1.0f;

		e->Setup(epos, dir);
		e->Scale(3.0f, 3.0f, 3.0f);
		e->SetColor(CColor(0.0f, 1.0f, 0.2, 1.0f));
	}
}
// �o���l���쐬(�傫���X���C���p)
void CExpManeger::SlimeBigSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// �o���l�I�[�u�̐�������ݒ�
	int spawnCount = exp;
	// ���������ŏ������傫���ꍇ�́A
	if (spawnCount > SlimeBig_SPAWN_EXP_COUNT_MIN)
	{
		// �����ł���ő吔���擾
		int maxCount = std::min(spawnCount, SlimeBig_SPAWN_EXP_COUNT_MAX);
		// ���������ŏ����`�ő吔�̃����_���Ŏ擾
		spawnCount = Math::Rand(SlimeBig_SPAWN_EXP_COUNT_MIN, maxCount);
	}

	// �J�n�p�x�̓����_���Ɍ��߂�
	float startAngle = Math::Rand(0.0f, M_PI * 2.0f);
	// ���Z����o���l�̃x�[�X�l
	int baseExp = exp / spawnCount;
	// ����؂�Ȃ��]��̌o���l
	int remainExp = exp % spawnCount;
	for (int i = 0; i < spawnCount; i++)
	{
		// �]��̌o���l���c���Ă���ꍇ�́A���Z����o���l�̃x�[�X�l+1����
		CExp* e = new CExp(baseExp + (i < remainExp ? 1 : 0));

		// ���Ԋu��360�x�o���l���o��������
		float per = (float)i / spawnCount;
		float angle = startAngle + M_PI * 2.0f * per;
		CVector dir = CVector(cosf(angle), 0.0f, sinf(angle));
		CVector epos = pos + dir * 1.0f;

		e->Setup(epos, dir);
		e->Scale(3.0f, 3.0f, 3.0f);
		e->SetColor(CColor(0.0f, 1.0f, 0.2, 1.0f));
	}
}

// �o���l���쐬(�������}�b�V�����[���p)
void CExpManeger::MushroomSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// �o���l�I�[�u�̐�������ݒ�
	int spawnCount = exp;
	// ���������ŏ������傫���ꍇ�́A
	if (spawnCount > Mushroom_SPAWN_EXP_COUNT_MIN)
	{
		// �����ł���ő吔���擾
		int maxCount = std::min(spawnCount, Mushroom_SPAWN_EXP_COUNT_MAX);
		// ���������ŏ����`�ő吔�̃����_���Ŏ擾
		spawnCount = Math::Rand(Mushroom_SPAWN_EXP_COUNT_MIN, maxCount);
	}

	// �J�n�p�x�̓����_���Ɍ��߂�
	float startAngle = Math::Rand(0.0f, M_PI * 2.0f);
	// ���Z����o���l�̃x�[�X�l
	int baseExp = exp / spawnCount;
	// ����؂�Ȃ��]��̌o���l
	int remainExp = exp % spawnCount;
	for (int i = 0; i < spawnCount; i++)
	{
		// �]��̌o���l���c���Ă���ꍇ�́A���Z����o���l�̃x�[�X�l+1����
		CExp* e = new CExp(baseExp + (i < remainExp ? 1 : 0));

		// ���Ԋu��360�x�o���l���o��������
		float per = (float)i / spawnCount;
		float angle = startAngle + M_PI * 2.0f * per;
		CVector dir = CVector(cosf(angle), 0.0f, sinf(angle));
		CVector epos = pos + dir * 1.0f;

		e->Setup(epos, dir);
		e->Scale(3.0f, 3.0f, 3.0f);
		e->SetColor(CColor(0.0f, 1.0f, 0.2, 1.0f));
	}
}
// �o���l���쐬(�傫���}�b�V�����[���p)
void CExpManeger::MushroomBigSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// �o���l�I�[�u�̐�������ݒ�
	int spawnCount = exp;
	// ���������ŏ������傫���ꍇ�́A
	if (spawnCount > MushroomBig_SPAWN_EXP_COUNT_MIN)
	{
		// �����ł���ő吔���擾
		int maxCount = std::min(spawnCount, MushroomBig_SPAWN_EXP_COUNT_MAX);
		// ���������ŏ����`�ő吔�̃����_���Ŏ擾
		spawnCount = Math::Rand(MushroomBig_SPAWN_EXP_COUNT_MIN, maxCount);
	}

	// �J�n�p�x�̓����_���Ɍ��߂�
	float startAngle = Math::Rand(0.0f, M_PI * 2.0f);
	// ���Z����o���l�̃x�[�X�l
	int baseExp = exp / spawnCount;
	// ����؂�Ȃ��]��̌o���l
	int remainExp = exp % spawnCount;
	for (int i = 0; i < spawnCount; i++)
	{
		// �]��̌o���l���c���Ă���ꍇ�́A���Z����o���l�̃x�[�X�l+1����
		CExp* e = new CExp(baseExp + (i < remainExp ? 1 : 0));

		// ���Ԋu��360�x�o���l���o��������
		float per = (float)i / spawnCount;
		float angle = startAngle + M_PI * 2.0f * per;
		CVector dir = CVector(cosf(angle), 0.0f, sinf(angle));
		CVector epos = pos + dir * 1.0f;

		e->Setup(epos, dir);
		e->Scale(3.0f, 3.0f, 3.0f);
		e->SetColor(CColor(0.0f, 1.0f, 0.2, 1.0f));
	}
}


// �o���l���쐬(���x��50�ȉ��̓G�p)
void CExpManeger::SpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// �o���l�I�[�u�̐�������ݒ�
	int spawnCount = exp;
	// ���������ŏ������傫���ꍇ�́A
	if (spawnCount > Level50_SPAWN_EXP_COUNT_MIN)
	{
		// �����ł���ő吔���擾
		int maxCount = std::min(spawnCount, Level50_SPAWN_EXP_COUNT_MAX);
		// ���������ŏ����`�ő吔�̃����_���Ŏ擾
		spawnCount = Math::Rand(Level50_SPAWN_EXP_COUNT_MIN, maxCount);
	}
	
	// �J�n�p�x�̓����_���Ɍ��߂�
	float startAngle = Math::Rand(0.0f, M_PI * 2.0f);
	// ���Z����o���l�̃x�[�X�l
	int baseExp = exp / spawnCount;
	// ����؂�Ȃ��]��̌o���l
	int remainExp = exp % spawnCount;
	for (int i = 0; i < spawnCount; i++)
	{
		// �]��̌o���l���c���Ă���ꍇ�́A���Z����o���l�̃x�[�X�l+1����
		CExp* e = new CExp(baseExp + (i < remainExp ? 1 : 0));

		// ���Ԋu��360�x�o���l���o��������
		float per = (float)i / spawnCount;
		float angle = startAngle + M_PI * 2.0f * per;
		CVector dir = CVector(cosf(angle), 0.0f, sinf(angle));
		CVector epos = pos + dir * 1.0f;

		e->Setup(epos, dir);
		e->Scale(3.0f, 3.0f, 3.0f);
		e->SetColor(CColor(0.0f, 1.0f, 0.2, 1.0f));
	}
}

// �o���l���쐬(���x��90�ȉ��̓G�p)
void CExpManeger::SpawnExp2(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// �o���l�I�[�u�̐�������ݒ�
	int spawnCount = exp;
	// ���������ŏ������傫���ꍇ�́A
	if (spawnCount > Level90_SPAWN_EXP_COUNT_MIN)
	{
		// �����ł���ő吔���擾
		int maxCount = std::min(spawnCount, Level90_SPAWN_EXP_COUNT_MAX);
		// ���������ŏ����`�ő吔�̃����_���Ŏ擾
		spawnCount = Math::Rand(Level90_SPAWN_EXP_COUNT_MIN, maxCount);
	}

	// �J�n�p�x�̓����_���Ɍ��߂�
	float startAngle = Math::Rand(0.0f, M_PI * 2.0f);
	// ���Z����o���l�̃x�[�X�l
	int baseExp = exp / spawnCount;
	// ����؂�Ȃ��]��̌o���l
	int remainExp = exp % spawnCount;
	for (int i = 0; i < spawnCount; i++)
	{
		// �]��̌o���l���c���Ă���ꍇ�́A���Z����o���l�̃x�[�X�l+1����
		CExp* e = new CExp(baseExp + (i < remainExp ? 1 : 0));

		// ���Ԋu��360�x�o���l���o��������
		float per = (float)i / spawnCount;
		float angle = startAngle + M_PI * 2.0f * per;
		CVector dir = CVector(cosf(angle), 0.0f, sinf(angle));
		CVector epos = pos + dir * 1.0f;

		e->Setup(epos, dir);
		e->Scale(3.0f, 3.0f, 3.0f);
		e->SetColor(CColor(0.0f, 1.0f, 0.2, 1.0f));
	}
}