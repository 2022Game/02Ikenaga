#pragma once
#include "CXCharacter.h"

// �G�l�~�[�̃x�[�X�N���X
class  CEnemy :public CXCharacter
{
public:
	//�@�R���X�g���N�^
	CEnemy();
	// �f�X�g���N�^
	virtual ~CEnemy();

	// ���S����
	virtual void Death();

private:
};