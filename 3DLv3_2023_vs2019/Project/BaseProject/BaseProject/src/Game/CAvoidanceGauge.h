#pragma once
#include "CUIBase.h"
#include "CVector.h"
class CImage;

/*
 ����Q�[�W
*/
class CAvoidanceGauge : public CUIBase
{
public:

	// �R���X�g���N�^
	CAvoidanceGauge(bool is3dGauge);

	// �f�X�g���N�^
	~CAvoidanceGauge();

	// ����Q�[�W���폜
	void Kill()override;

	// �\�����邩�ǂ����ݒ�
	void SetShow(bool isShow)override;

	// �ő�l��ݒ�
	void SetMaxValue(int value);

	// ���ݒl��ݒ�
	void SetValue(int value);

	// ���S�ʒu�̂̊����̐ݒ�
	void SetCenterRatio(const CVector2& ratio);

	// ���[���h���W��ݒ�
	void SetWorldPos(const CVector& worldPos);

	// �X�V
	void Update();

private:

	CImage* mpFrameImage;  // �Q�[�W�̃t���[���̃C���[�W
	CImage* mpBarImage;    // �Q�[�W�̃o�[�̃C���[�W	
	CImage* mpEdgeImage;   // �Q�[�W�̂ӂ�
	int mMaxValue;         // �|�C���g�̍ő�l
	int mValue;            // �|�C���g�̌��ݒl
	CVector2 mCenterRatio; // ���S�ʒu�̊���
	float mScale;          // ����Q�[�W�̃X�P�[���l
	bool mIs3dGauge;       // 3D��Ԃɔz�u����Q�[�W���ǂ���
};