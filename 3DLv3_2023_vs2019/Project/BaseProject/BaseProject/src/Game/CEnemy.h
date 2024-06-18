#pragma once
#include "CXCharacter.h"
#include "CHpGauge.h"

// �G�̎��
enum class EEnemyType
{
	None = -1,

	eBee,
	eBeholder,
	eBoxer,
	eCactus,
	eChest,
	eDragon,
	eMushroom,
	eRay,
	eRich,
	eSlime,
	eTurtle,

	Num
};

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

	// �������T�̎��S����
	virtual void TurtleDeath();
	// �傫���T�̎��S����
	virtual void TurtleBigDeath();

	// �������G�C�̎��S����
	virtual void RayDeath();
	// �傫���G�C�̎��S����
	virtual void RayBigDeath();

	// �������I�̎��S����
	virtual void BeeDeath();
	// �傫���I�̎��S����
	virtual void BeeBigDeath();

	// �������T�{�e���̎��S����
	virtual void CactusDeath();
	// �傫���T�{�e���̎��S����
	virtual void CactusBigDeath();

	// �������`�F�X�g�����X�^�[�̎��S����
	virtual void ChestDeath();
	// �傫���`�F�X�g�����X�^�[�̎��S����
	virtual void ChestBigDeath();

	// ���������̃����X�^�[�̎��S����
	virtual void BeholderDeath();
	// �傫�����̃����X�^�[�̎��S����
	virtual void BeholderBigDeath();

	// �������{�N�T�[�̎��S����
	virtual void BoxerDeath();
	// �傫���{�N�T�[�̎��S����
	virtual void BoxerBigDeath();

	// �{�X(�h���S��)�̎��S����
	virtual void DragonDeath();

	// ���S����
	virtual void Death();

	// �G�̎�ނ��擾
	EEnemyType GetType() const;

	// ���������G�̎��S����
	virtual void DeathSummonEnemy(CEnemy* enemy);
	// ���g�����������G��ݒ�
	void SetSummoner(CEnemy* summoner);

protected:
	EEnemyType mType;     // �G�̎��
	CHpGauge* mpHpGauge;  // HP�Q�[�W

	CEnemy* mpSummoner;   // �������g�����������G�L����
};