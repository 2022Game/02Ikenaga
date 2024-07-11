#pragma once
#include "CUIBase.h"

class CText;

// SP��UI
class CSpUI : public CUIBase
{
public:

	// �R���X�g���N
	CSpUI();

	// �f�X�g���N�^
	~CSpUI();

	// Sp��ݒ�
	void SetSpText(std::string spText);

	// �`��
	void Render()override;
private:
	CText* mpSpText;    // Sp�e�L�X�g
	CVector2 mBasePos;  // �x�[�X�ʒu
};