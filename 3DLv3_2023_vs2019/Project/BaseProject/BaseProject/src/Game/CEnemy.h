#pragma once
#include "CXCharacter.h"
#include "CHpGauge.h"

// �G�l�~�[�̃x�[�X�N���X
class  CEnemy :public CXCharacter
{
public:
	//�@�R���X�g���N�^
	CEnemy();
	// �f�X�g���N�^
	virtual ~CEnemy();

	// �������X���C���̎��S����
	virtual void SlimeDeath();
	// �傫���X���C���̎��S����
	virtual void SlimeBigDeath();

	// �������}�b�V�����[���̎��S����
	virtual void MushroomDeath();
	// �傫���}�b�V�����[���̎��S����
	virtual void MushroomBigDeath();

	// ���S����(���x��50�ȉ��̓G�p)
	virtual void Death();

	// ���S����(���x��90�ȉ��̓G�p)
	virtual void Death2();
protected:
	CHpGauge* mpHpGauge;      // HP�Q�[�W
private:
};