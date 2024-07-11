#pragma once
#include "CUIBase.h"

class CText;

// HP��UI
class CHpUI : public CUIBase
{
public:

	// �R���X�g���N
	CHpUI();

	// �f�X�g���N�^
	~CHpUI();

	// Hp��ݒ�
	void SetHpText(std::string hpText);

	// �`��
	void Render()override;
private:
	CText* mpHpText;    // Hp�e�L�X�g
	CVector2 mBasePos;  // �x�[�X�̈ʒu
};