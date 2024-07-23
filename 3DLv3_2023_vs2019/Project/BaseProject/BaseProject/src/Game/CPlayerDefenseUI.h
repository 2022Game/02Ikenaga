#pragma once
#include "CUIBase.h"

class CText;

// �h��͂�UI
class CPlayerDefenseUI : public CUIBase
{
public:

	// �R���X�g���N
	CPlayerDefenseUI(float x, float y);

	// �f�X�g���N�^
	~CPlayerDefenseUI();

	// �h��͂�ݒ�
	void SetDefense(int defense);

	// �`��
	void Render()override;
private:
	CText* mpDefenseText;  // �h��͂̃e�L�X�g
	CVector2 mBasePos;     // �x�[�X�̈ʒu
};