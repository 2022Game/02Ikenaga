#pragma once
#include "CExp.h"

class CExpManeger :public CTask
{
public:
	// �o���l���쐬
	static void SpawnExp(int exp, const CVector& pos);

private:
	// �R���X�g���N�^
	CExpManeger();
	//�f�X�g���N�^
	~CExpManeger();

	static CExpManeger* spInstance;
};