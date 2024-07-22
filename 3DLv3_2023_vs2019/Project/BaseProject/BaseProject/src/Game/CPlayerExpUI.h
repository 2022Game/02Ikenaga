#pragma once
#include "CUIBase.h"

class CText;

// �v���C���[Exp��UI
class CPlayerExpUI : public CUIBase
{
public:

	// �R���X�g���N
	CPlayerExpUI(float x, float y);

	// �f�X�g���N�^
	~CPlayerExpUI();

	// �v���C���[EXP��ݒ�
	void SetExp(int exp);

	// �`��
	void Render()override;
private:
	CText* mpExpText;
	CVector2 mBasePos;
};
