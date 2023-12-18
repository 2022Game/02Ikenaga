#pragma once
#include "CUIBase.h"
class CBillBoardImage;

/*
�G��HP�N���X
�I�u�W�F�N�g�x�[�X�p��
*/
class CHpGauge2 : public CUIBase
{
private:
	CBillBoardImage* mpBarImage;    //�Q�[�W�̃o�[�̃C���[�W	
	CBillBoardImage* mpFrameImage;  //�Q�[�W�̃t���[���̃C���[�W
	int mMaxValue;         //�|�C���g�̍ő�l
	int mValue;            //�|�C��
	CVector mMoveSpeed;  //�ړ����x

public:
	CHpGauge2();
	~CHpGauge2();

	// �����ݒ�
	void Setup(const CVector& pos, const CVector& dir);

	//�ő�l��ݒ�
	void SetMaxValue(int value);
	//���ݒl��ݒ�
	void SetValue(int value);

	void Update()override;
};