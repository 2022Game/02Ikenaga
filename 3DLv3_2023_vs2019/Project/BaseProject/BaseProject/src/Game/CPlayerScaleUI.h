#pragma once
#include "CUIBase.h"

class CText;

// �傫����UI
class CPlayerScaleUI : public CUIBase
{
public:

	// �R���X�g���N
	CPlayerScaleUI(float x, float y);

	// �f�X�g���N�^
	~CPlayerScaleUI();

	// �傫����ݒ�
	void SetScale(float scale);

	// �`��
	void Render()override;
private:
	CText* mpScaleText;  // �U���͂̃e�L�X�g
	CVector2 mBasePos;   // �x�[�X�̈ʒu
};