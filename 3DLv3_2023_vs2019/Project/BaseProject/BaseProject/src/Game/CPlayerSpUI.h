#pragma once
#include "CUIBase.h"

class CText;

// �v���C���[SP��UI
class CPlayerSpUI : public CUIBase
{
public:

	// �R���X�g���N
	CPlayerSpUI(float x, float y);

	// �f�X�g���N�^
	~CPlayerSpUI();

	// �v���C���[SP��ݒ�
	void SetSp(int sp);

	// �`��
	void Render()override;
private:
	CText* mpSpText;
	CVector2 mBasePos;
};