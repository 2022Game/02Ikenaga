#pragma once
#include "CUIBase.h"

class CText;

// �v���C���[�̍ő�SP��UI
class CPlayerMaxSpUI : public CUIBase
{
public:

	// �R���X�g���N
	CPlayerMaxSpUI(float x, float y);

	// �f�X�g���N�^
	~CPlayerMaxSpUI();

	// �v���C���[�̍ő�SP��ݒ�
	void SetMaxSp(int maxSp);

	// �`��
	void Render()override;
private:
	CText* mpMaxSpText;
	CVector2 mBasePos;
};