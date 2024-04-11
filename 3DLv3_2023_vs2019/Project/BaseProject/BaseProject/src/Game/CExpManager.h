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