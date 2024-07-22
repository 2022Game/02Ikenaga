#pragma once
#include "CUIBase.h"

class CText;

// �v���C���[�ő�EXP��UI
class CPlayerMaxExpUI : public CUIBase
{
public:

	// �R���X�g���N
	CPlayerMaxExpUI(float x, float y);

	// �f�X�g���N�^
	~CPlayerMaxExpUI();

	// �v���C���[�ő�EXP��ݒ�
	void SetMaxExp(int maxExp);

	// �`��
	void Render()override;
private:
	CText* mpMaxExpText;
	CVector2 mBasePos;
};