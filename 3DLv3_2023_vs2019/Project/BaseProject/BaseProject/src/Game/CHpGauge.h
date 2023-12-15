#pragma once
#include "CUIBase.h"
class CImage;

/*
HP�Q�[�W
*/
class CHpGauge : public CUIBase
{
private:
	CImage* mpFrameImage;  //�Q�[�W�̃t���[���̃C���[�W
	CImage* mpBarImage;    //�Q�[�W�̃o�[�̃C���[�W	
	int mMaxValue;         //�|�C���g�̍ő�l
	int mValue;            //�|�C���g�̌��ݒl


public:
	//�R���X�g���N�^
	CHpGauge();
	//�f�X�g���N�^
	~CHpGauge();

	//�ő�l��ݒ�
	void SetMaxValue(int value);
	//���ݒl��ݒ�
	void SetValue(int value);

	//�X�V
	void Update();
};