#pragma once
#include "CUIBase.h"
class CImage;

/*
�X�y�V�����|�C���g�Q�[�W
*/
class CSpGauge : public CUIBase
{
public:

	// �R���X�g���N�^
	CSpGauge();

	// �f�X�g���N�^
	~CSpGauge();

	// �ő�l��ݒ�
	void SetMaxValue(int value);

	// ���ݒl��ݒ�
	void SetValue(int value);

	// �X�V
	void Update();

private:

	CImage* mpFrameImage;  // �Q�[�W�̃t���[���̃C���[�W
	CImage* mpBarImage;    // �Q�[�W�̃o�[�̃C���[�W	
	CImage* mpEdgeImage;   // �Q�[�W�̂ӂ�
	int mMaxValue;         // �|�C���g�̍ő�l
	int mValue;            // �|�C���g�̌��ݒl
};
