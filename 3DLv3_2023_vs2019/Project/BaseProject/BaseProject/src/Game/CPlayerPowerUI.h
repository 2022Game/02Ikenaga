#pragma once
#include "CUIBase.h"

class CText;

// �U���͂�UI
class CPlayerPowerUI : public CUIBase
{
public:

	// �R���X�g���N
	CPlayerPowerUI(float x, float y);

	// �f�X�g���N�^
	~CPlayerPowerUI();

	// �U���͂�ݒ�
	void SetPower(int power);

	// �`��
	void Render()override;
private:
	CText* mpPowerText;   // �U���͂̃e�L�X�g
	CVector2 mBasePos;  // �x�[�X�̈ʒu
};