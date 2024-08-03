#pragma once
#include <initializer_list>

// �I�u�W�F�N�g���ʗp�̃^�O
enum class ETag
{
	eNone = -1,

	eField,	          // �t�B�[���h���\������I�u�W�F�N�g
	eSky,             // ��
	eExp,             // �o���l
	eRideableObject,  // ��邱�Ƃ��ł���I�u�W�F�N�g
	ePortionRed,	  // �A�C�e���̍U���̓A�b�v
	ePortionGreen,    // �A�C�e���̉�
	ePortionBlue,     // �A�C�e���̖h��̓A�b�v
	eWeapon,          // ����

	ePlayer,          // �v���C���[
	eEnemy,           // �G�l�~�[
	eBullet,          // �e��
	eFlame,	          // ��
	eSlash,	          // �X���b�V��
	eWave,            // �g��
	eNeedle,          // �j
	eLightningBall,   // ����
	eElectricShock,   // �d��
	eImpact,          // �Ռ�
	eHomingBall,      // �z�[�~���O�{�[��
	eDrain,           // �h���C��(HP�z��)
	eAura,            // �I�[��
	eCurrent,         // �d��
	eBreath,          // �u���X
	eHit,             // �q�b�g
	eDizzy,           // �߂܂�

	eCamera,          // �J����
	eUI,              // UI

	Num
};
// �I�u�W�F�N�g���ʗp�̃^�O�̏��������X�g
using Tags = std::initializer_list<ETag>;
