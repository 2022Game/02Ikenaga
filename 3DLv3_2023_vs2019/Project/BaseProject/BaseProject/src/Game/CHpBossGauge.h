#pragma once
#include "CUIBase.h"
#include "CVector.h"
class CImage;

/*
HP�Q�[�W
*/
class CHpBossGauge : public CUIBase
{
public:

	// �R���X�g���N�^
	CHpBossGauge(bool is3dGauge);

	// �f�X�g���N�^
	~CHpBossGauge();

	// HP�Q�[�W���폜
	void Kill()override;

	// �\�����邩�ǂ����ݒ�
	void SetShow(bool isShow)override;

	// �ő�l��ݒ�
	void SetMaxHpValue(int value);

	// ���ݒl��ݒ�
	void SetHpValue(int value);

	// ���S�ʒu�̂̊����̐ݒ�
	void SetCenterRatio(const CVector2& ratio);

	// ���[���h���W��ݒ�
	void SetWorldPos(const CVector& worldPos);

	// �X�V
	void Update();

private:

	CImage* mpFrameImage;      // �Q�[�W�̃t���[���̃C���[�W
	CImage* mpDamagaBarImage;  // �_���[�W�o�[�̃C���[�W
	CImage* mpHpBarImage;      // HP�Q�[�W�̃o�[�̃C���[�W
	CImage* mpEdgeImage;       // �Q�[�W�̂ӂ�

	int mMaxValue;             // �|�C���g�̍ő�l
	int mValue;                // �|�C���g�̌��ݒl

	CVector2 mCenterRatio;     // ���S�ʒu�̊���
	float mScale;              // HP�Q�[�W�̃X�P�[���l
	bool mIs3dGauge;           // 3D��Ԃɔz�u����Q�[�W���ǂ���

	int mDamageValue;          // �_���[�W�o�[�\���p�̃|�C���g
	float mDamageWaitTime;     // �_���[�W�o�[����������܂ł̎c�莞��
	float mDamageElapsedTime;  // �_���[�W�o�[�̌������̌o�ߎ���
};