#pragma once
#include "CUIBase.h"
class CImage;

/*
�X�y�V�����A�^�b�N�Q�[�W
*/
class CSaGauge : public CUIBase
{
private:
	CImage* mpFrameImage;  //�Q�[�W�̃t���[���̃C���[�W
	CImage* mpBarImage;    //�Q�[�W�̃o�[�̃C���[�W	
	int mMaxValue;         //�|�C���g�̍ő�l
	int mValue;            //�|�C���g�̌��ݒl

public:
	//�R���X�g���N�^
	CSaGauge();
	//�f�X�g���N�^
	~CSaGauge();

	//�ő�l��ݒ�
	void SetMaxValue(int value);
	//���ݒl��ݒ�
	void SetValue(int value);

	//�X�V
	void Update();
};
