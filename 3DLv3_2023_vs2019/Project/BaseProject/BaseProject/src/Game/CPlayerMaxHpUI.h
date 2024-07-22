#pragma once
#include "CUIBase.h"

class CText;

// �v���C���[�ő�HP��UI
class CPlayerMaxHpUI : public CUIBase
{
public:

	// �R���X�g���N
	CPlayerMaxHpUI(float x, float y);

	// �f�X�g���N�^
	~CPlayerMaxHpUI();

	// �v���C���[�ő�HP��ݒ�
	void SetMaxHp(int hp);

	// �`��
	void Render()override;
private:
	CText* mpMaxHpText;
	CVector2 mBasePos;
};