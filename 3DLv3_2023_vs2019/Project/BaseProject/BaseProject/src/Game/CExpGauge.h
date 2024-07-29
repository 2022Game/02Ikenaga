#pragma once
#include "CUIBase.h"
class CImage;

/*
 �o���l�̃Q�[�W
*/
class CExpGauge : public CUIBase
{
public:

	// �R���X�g���N�^
	CExpGauge();

	// �f�X�g���N�^
	~CExpGauge();

	// �ő�l��ݒ�
	void SetMaxValue(int value);

	// ���ݒl��ݒ�
	void SetValue(int value);

	// �\�����邩�ǂ����ݒ�
	void SetShow(bool isShow)override;

	// �X�V
	void Update();

private:

	CImage* mpBarImage;    // �Q�[�W�̃o�[
	CImage* mpEdgeImage;   // �Q�[�W�̂ӂ�
	int mMaxValue;         // �|�C���g�̍ő�l
	int mValue;            // �|�C���g�̌��ݒl
};