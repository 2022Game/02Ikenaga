#pragma once
#include "CUIBase.h"
#include "CVector.h"
class CImage;

/*
HP�Q�[�W
*/
class CHpGauge : public CUIBase
{
private:
	CImage* mpFrameImage;  // �Q�[�W�̃t���[���̃C���[�W
	CImage* mpBarImage;    // �Q�[�W�̃o�[�̃C���[�W	
	CImage* mpEdgeImage;   // �Q�[�W�̂ӂ�
	int mMaxValue;         // �|�C���g�̍ő�l
	int mValue;            // �|�C���g�̌��ݒl
	CVector2 mCenterRatio; // ���S�ʒu�̊���
	float mScale;          // HP�Q�[�W�̃X�P�[���l

public:
	// �R���X�g���N�^
	CHpGauge();
	// �f�X�g���N�^
	~CHpGauge();

	// HP�Q�[�W���폜
	void Kill()override;

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
};