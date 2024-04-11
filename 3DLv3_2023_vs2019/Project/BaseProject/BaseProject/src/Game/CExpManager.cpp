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

// �������T�p
#define Turtle_SPAWN_EXP_COUNT_MIN 3  // ��������o���l�I�[�u�̍ŏ���
#define Turtle_SPAWN_EXP_COUNT_MAX 4  // ��������o���l�I�[�u�̍ő吔
// �傫���T�p
#define TurtleBig_SPAWN_EXP_COUNT_MIN 5  // ��������o���l�I�[�u�̍ŏ���
#define TurtleBig_SPAWN_EXP_COUNT_MAX 6  // ��������o���l�I�[�u�̍ő吔

// �������G�C�p
#define Ray_SPAWN_EXP_COUNT_MIN 4  // ��������o���l�I�[�u�̍ŏ���
#define Ray_SPAWN_EXP_COUNT_MAX 5  // ��������o���l�I�[�u�̍ő吔
// �傫���G�C�p
#define RayBig_SPAWN_EXP_COUNT_MIN 6  // ��������o���l�I�[�u�̍ŏ���
#define RayBig_SPAWN_EXP_COUNT_MAX 7  // ��������o���l�I�[�u�̍ő吔

// �������I�p
#define Bee_SPAWN_EXP_COUNT_MIN 5  // ��������o���l�I�[�u�̍ŏ���
#define Bee_SPAWN_EXP_COUNT_MAX 6  // ��������o���l�I�[�u�̍ő吔
// �傫���I�p
#define BeeBig_SPAWN_EXP_COUNT_MIN 7  // ��������o���l�I�[�u�̍ŏ���
#define BeeBig_SPAWN_EXP_COUNT_MAX 8  // ��������o���l�I�[�u�̍ő吔

// �������T�{�e���p
#define Cactus_SPAWN_EXP_COUNT_MIN 6  // ��������o���l�I�[�u�̍ŏ���
#define Cactus_SPAWN_EXP_COUNT_MAX 7  // ��������o���l�I�[�u�̍ő吔
// �傫���T�{�e���p
#define CactusBig_SPAWN_EXP_COUNT_MIN 8  // ��������o���l�I�[�u�̍ŏ���
#define CactusBig_SPAWN_EXP_COUNT_MAX 9  // ��������o���l�I�[�u�̍ő吔

// �������`�F�X�g�����X�^�[�p
#define Chest_SPAWN_EXP_COUNT_MIN 7  // ��������o���l�I�[�u�̍ŏ���
#define Chest_SPAWN_EXP_COUNT_MAX 8  // ��������o���l�I�[�u�̍ő吔
// �傫���`�F�X�g�����X�^�[�p
#define ChestBig_SPAWN_EXP_COUNT_MIN 9   // ��������o���l�I�[�u�̍ŏ���
#define ChestBig_SPAWN_EXP_COUNT_MAX 10  // ��������o���l�I�[�u�̍ő吔

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
	// ���������ŏ������傫���ꍇ��
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
	// ���������ŏ������傫���ꍇ��
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
	// ���������ŏ������傫���ꍇ��
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
	// ���������ŏ������傫���ꍇ��
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

// �o���l���쐬(�������T�p)
void CExpManeger::TurtleSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// �o���l�I�[�u�̐�������ݒ�
	int spawnCount = exp;
	// ���������ŏ������傫���ꍇ��
	if (spawnCount > Turtle_SPAWN_EXP_COUNT_MIN)
	{
		// �����ł���ő吔���擾
		int maxCount = std::min(spawnCount, Turtle_SPAWN_EXP_COUNT_MAX);
		// ���������ŏ����`�ő吔�̃����_���Ŏ擾
		spawnCount = Math::Rand(Turtle_SPAWN_EXP_COUNT_MIN, maxCount);
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
// �o���l���쐬(�傫���T�p)
void CExpManeger::TurtleBigSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// �o���l�I�[�u�̐�������ݒ�
	int spawnCount = exp;
	// ���������ŏ������傫���ꍇ��
	if (spawnCount > TurtleBig_SPAWN_EXP_COUNT_MIN)
	{
		// �����ł���ő吔���擾
		int maxCount = std::min(spawnCount, TurtleBig_SPAWN_EXP_COUNT_MAX);
		// ���������ŏ����`�ő吔�̃����_���Ŏ擾
		spawnCount = Math::Rand(TurtleBig_SPAWN_EXP_COUNT_MIN, maxCount);
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

// �o���l���쐬(�������G�C�p)
void CExpManeger::RaySpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// �o���l�I�[�u�̐�������ݒ�
	int spawnCount = exp;
	// ���������ŏ������傫���ꍇ��
	if (spawnCount > Ray_SPAWN_EXP_COUNT_MIN)
	{
		// �����ł���ő吔���擾
		int maxCount = std::min(spawnCount, Ray_SPAWN_EXP_COUNT_MAX);
		// ���������ŏ����`�ő吔�̃����_���Ŏ擾
		spawnCount = Math::Rand(Ray_SPAWN_EXP_COUNT_MIN, maxCount);
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
// �o���l���쐬(�傫���G�C�p)
void CExpManeger::RayBigSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// �o���l�I�[�u�̐�������ݒ�
	int spawnCount = exp;
	// ���������ŏ������傫���ꍇ��
	if (spawnCount > RayBig_SPAWN_EXP_COUNT_MIN)
	{
		// �����ł���ő吔���擾
		int maxCount = std::min(spawnCount, RayBig_SPAWN_EXP_COUNT_MAX);
		// ���������ŏ����`�ő吔�̃����_���Ŏ擾
		spawnCount = Math::Rand(RayBig_SPAWN_EXP_COUNT_MIN, maxCount);
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

// �o���l���쐬(�������I�p)
void CExpManeger::BeeSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// �o���l�I�[�u�̐�������ݒ�
	int spawnCount = exp;
	// ���������ŏ������傫���ꍇ��
	if (spawnCount > Bee_SPAWN_EXP_COUNT_MIN)
	{
		// �����ł���ő吔���擾
		int maxCount = std::min(spawnCount, Bee_SPAWN_EXP_COUNT_MAX);
		// ���������ŏ����`�ő吔�̃����_���Ŏ擾
		spawnCount = Math::Rand(Bee_SPAWN_EXP_COUNT_MIN, maxCount);
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
// �o���l���쐬(�傫���I�p)
void CExpManeger::BeeBigSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// �o���l�I�[�u�̐�������ݒ�
	int spawnCount = exp;
	// ���������ŏ������傫���ꍇ��
	if (spawnCount > BeeBig_SPAWN_EXP_COUNT_MIN)
	{
		// �����ł���ő吔���擾
		int maxCount = std::min(spawnCount, BeeBig_SPAWN_EXP_COUNT_MAX);
		// ���������ŏ����`�ő吔�̃����_���Ŏ擾
		spawnCount = Math::Rand(BeeBig_SPAWN_EXP_COUNT_MIN, maxCount);
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

// �o���l���쐬(�������T�{�e���p)
void CExpManeger::CactusSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// �o���l�I�[�u�̐�������ݒ�
	int spawnCount = exp;
	// ���������ŏ������傫���ꍇ��
	if (spawnCount > Cactus_SPAWN_EXP_COUNT_MIN)
	{
		// �����ł���ő吔���擾
		int maxCount = std::min(spawnCount, Cactus_SPAWN_EXP_COUNT_MAX);
		// ���������ŏ����`�ő吔�̃����_���Ŏ擾
		spawnCount = Math::Rand(Cactus_SPAWN_EXP_COUNT_MIN, maxCount);
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
// �o���l���쐬(�傫���T�{�e���p)
void CExpManeger::CactusBigSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// �o���l�I�[�u�̐�������ݒ�
	int spawnCount = exp;
	// ���������ŏ������傫���ꍇ��
	if (spawnCount > CactusBig_SPAWN_EXP_COUNT_MIN)
	{
		// �����ł���ő吔���擾
		int maxCount = std::min(spawnCount, CactusBig_SPAWN_EXP_COUNT_MAX);
		// ���������ŏ����`�ő吔�̃����_���Ŏ擾
		spawnCount = Math::Rand(CactusBig_SPAWN_EXP_COUNT_MIN, maxCount);
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

// �o���l���쐬(�������`�F�X�g�����X�^�[�p)
void CExpManeger::ChestSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// �o���l�I�[�u�̐�������ݒ�
	int spawnCount = exp;
	// ���������ŏ������傫���ꍇ��
	if (spawnCount > Chest_SPAWN_EXP_COUNT_MIN)
	{
		// �����ł���ő吔���擾
		int maxCount = std::min(spawnCount, Chest_SPAWN_EXP_COUNT_MAX);
		// ���������ŏ����`�ő吔�̃����_���Ŏ擾
		spawnCount = Math::Rand(Chest_SPAWN_EXP_COUNT_MIN, maxCount);
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
// �o���l���쐬(�傫���`�F�X�g�����X�^�[�p)
void CExpManeger::ChestBigSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// �o���l�I�[�u�̐�������ݒ�
	int spawnCount = exp;
	// ���������ŏ������傫���ꍇ��
	if (spawnCount > ChestBig_SPAWN_EXP_COUNT_MIN)
	{
		// �����ł���ő吔���擾
		int maxCount = std::min(spawnCount, ChestBig_SPAWN_EXP_COUNT_MAX);
		// ���������ŏ����`�ő吔�̃����_���Ŏ擾
		spawnCount = Math::Rand(ChestBig_SPAWN_EXP_COUNT_MIN, maxCount);
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