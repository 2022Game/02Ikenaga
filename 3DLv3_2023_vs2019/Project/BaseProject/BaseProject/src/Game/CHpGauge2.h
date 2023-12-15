#pragma once
#include "CObjectBase.h"
#include "CColliderSphere.h"
#include "CBillBoardImage.h"
class CImage;

/*
�G��HP�N���X
�I�u�W�F�N�g�x�[�X�p��
*/
class CHpGauge2 : public CBillBoardImage
{
private:
	CImage* mpFrameImage;  //�Q�[�W�̃t���[���̃C���[�W
	CImage* mpBarImage;    //�Q�[�W�̃o�[�̃C���[�W	
	int mMaxValue;         //�|�C���g�̍ő�l
	int mValue;            //�|�C��

public:
	CHpGauge2();
	~CHpGauge2();

	// �����ݒ�
	void Setup(const CVector& pos, const CVector& dir);

	//�ő�l��ݒ�
	void SetMaxValue(int value);
	//���ݒl��ݒ�
	void SetValue(int value);


	void Update() override;
};