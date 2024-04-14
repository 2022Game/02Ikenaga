#pragma once
#include "CExp.h"

/*
 �G�����Ƃ��o���l
*/
class CExpManeger :public CTask
{
public:
	// �o���l���쐬(�������X���C���p)
	static void SlimeSpawnExp(int exp, const CVector& pos);
	// �o���l���쐬(�傫���X���C���p)
	static void SlimeBigSpawnExp(int exp, const CVector& pos);

	// �o���l���쐬(�������}�b�V�����[���p)
	static void MushroomSpawnExp(int exp, const CVector& pos);
	// �o���l���쐬(�傫���}�b�V�����[���p)
	static void MushroomBigSpawnExp(int exp, const CVector& pos);

	// �o���l���쐬(�������T�p)
	static void TurtleSpawnExp(int exp, const CVector& pos);
	// �o���l���쐬(�傫���T�p)
	static void TurtleBigSpawnExp(int exp, const CVector& pos);

	// �o���l���쐬(�������G�C�p)
	static void RaySpawnExp(int exp, const CVector& pos);
	// �o���l���쐬(�傫���G�C�p)
	static void RayBigSpawnExp(int exp, const CVector& pos);

	// �o���l���쐬(�������I�p)
	static void BeeSpawnExp(int exp, const CVector& pos);
	// �o���l���쐬(�傫���I�p)
	static void BeeBigSpawnExp(int exp, const CVector& pos);

	// �o���l���쐬(�������T�{�e���p)
	static void CactusSpawnExp(int exp, const CVector& pos);
	// �o���l���쐬(�傫���T�{�e���p)
	static void CactusBigSpawnExp(int exp, const CVector& pos);

	// �o���l���쐬(�������`�F�X�g�����X�^�[�p)
	static void ChestSpawnExp(int exp, const CVector& pos);
	// �o���l���쐬(�傫���`�F�X�g�����X�^�[�p)
	static void ChestBigSpawnExp(int exp, const CVector& pos);

	// �o���l���쐬(���������̃����X�^�[�p)
	static void BeholderSpawnExp(int exp, const CVector& pos);
	// �o���l���쐬(�傫�����̃����X�^�[�p)
	static void BeholderBigSpawnExp(int exp, const CVector& pos);

	// �o���l���쐬(�������{�N�T�[�p)
	static void BoxerSpawnExp(int exp, const CVector& pos);
	// �o���l���쐬(�傫���{�N�T�[�p)
	static void BoxerBigSpawnExp(int exp, const CVector& pos);

	// �o���l���쐬(���x��50�ȉ��̓G�p)
	static void SpawnExp(int exp, const CVector& pos);
	// �o���l���쐬(���x��90�ȉ��̓G�p)
	static void SpawnExp2(int exp, const CVector& pos);

private:
	// �R���X�g���N�^
	CExpManeger();
	//�f�X�g���N�^
	~CExpManeger();

	static CExpManeger* spInstance;
};