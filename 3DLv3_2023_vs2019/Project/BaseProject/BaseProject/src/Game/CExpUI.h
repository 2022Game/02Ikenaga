#pragma once
#include "CUIBase.h"

class CText;

// Exp��UI
class CExpUI : public CUIBase
{
public:

	// �R���X�g���N
	CExpUI();

	// �f�X�g���N�^
	~CExpUI();

	// Exp��ݒ�
	void SetExpText(std::string expText);

	// �`��
	void Render()override;
private:
	CText* mpExpText;   // Exp�e�L�X�g
	CVector2 mBasePos;  // �x�[�X�̈ʒu
};