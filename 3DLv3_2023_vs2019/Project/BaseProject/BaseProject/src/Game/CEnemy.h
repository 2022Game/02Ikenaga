#pragma once
#include "CXCharacter.h"
#include "CHpGauge.h"

// �G�l�~�[�̃x�[�X�N���X(���x��50�܂ł̓G)
class  CEnemy :public CXCharacter
{
public:
	//�@�R���X�g���N�^
	CEnemy();
	// �f�X�g���N�^
	virtual ~CEnemy();

	// ���S����
	virtual void Death();
protected:
	CHpGauge* mpHpGauge;      // HP�Q�[�W
private:
};