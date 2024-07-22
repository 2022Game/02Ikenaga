#pragma once
#include "CUIBase.h"

class CText;

// �v���C���[HP��UI
class CPlayerHpUI : public CUIBase
{
public:

	// �R���X�g���N
	CPlayerHpUI(float x, float y);

	// �f�X�g���N�^
	~CPlayerHpUI();

	// �v���C���[HP��ݒ�
	void SetHp(int hp);

	// �`��
	void Render()override;
private:
	CText* mpHpText;
	CVector2 mBasePos;
};
